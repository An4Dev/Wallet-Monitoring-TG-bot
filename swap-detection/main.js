const fs = require('fs');
const path = require('path');
const WebSocket = require('ws');

const https = require('https');
const config = require('./config');

// Path to whales_address.txt file (two directories up from src to reach www)
const WHALES_ADDRESS_FILE = path.join(__dirname, '..','whales_address.txt');
const SWAP_ADDRESS_FILE = path.join(__dirname, '..','swap_address.txt');
const REJECTED_ADDRESSES_FILE = path.join(__dirname, '..','rejected_addresses.txt');

const TELEGRAM_BOT_TOKEN = config.TELEGRAM_BOT_TOKEN;
const TELEGRAM_CHAT_ID = config.TELEGRAM_CHAT_ID;

let SOLANA_ACCOUNTS = []; // Only enrolled (qualified) addresses

// Persistent set of rejected addresses to avoid re-fetching them
let REJECTED_ADDRESSES = new Set();

// Process management for three-process architecture
let isWhaleDetectionRunning = false;
let isBlacklistProcessingRunning = false;
let isSwapMonitoringRunning = false;

// Address processing queue and state
let addressQueue = [];
let processingQueue = new Set();
let qualifiedAddresses = new Set();
let rejectedAddresses = new Set();

// Batch processing configuration
const BATCH_SIZE = 5;
const BATCH_DELAY = 200; // ms between batches
const SWAP_UPDATE_INTERVAL = 4 * 60 * 1000; //4minutes

const MAX_PRIOR_TX = (config.NOTIFICATION_SETTINGS && config.NOTIFICATION_SETTINGS.MAX_PRIOR_TX) || 20;
const MAX_PRIOR_SWAPS = (config.NOTIFICATION_SETTINGS && config.NOTIFICATION_SETTINGS.MAX_PRIOR_SWAPS) || 5;

// WebSocket endpoint with API key
const WS_URL = `wss://atlas-mainnet.helius-rpc.com/?api-key=${API_KEY}`;

const processedTransactions = new Set();

let currentBatchSubscriptionId = null;

// DEX addresses for classification
const DEX_ADDRESSES = {
  // Jupiter Aggregator
  'JUP6LkbZbjS1jKKwapdHNy74zcZ3tLUZoi5QNyVTaV4': 'Jupiter',
  // OKX Aggregator
  '6m2CDdhRgxpH4WjvdzxAYbGxwdGUz5MziiL5jek2kBma': 'OKX',
  // Raydium
  'CAMMCzo5YL8w4VFF8KVHrK22GGUsp5VTaW7grrKgrWqK': 'RAYDIUM_CLMM',
  'CPMMoo8L3F4NbTegBCKVNunggL7H1ZpdTHKxQB5qKP1C': 'RAYDIUM_CPMM',
  '675kPX9MHTjS2zt1qfr1NYHuzeLXfQM9H24wFSUt1Mp8' : 'RAYDIUM_LIQUIDITY_POOL',
  'GpMZbSM2GgvTKHJirzeGfMFoaZ8UR2X7F4v8vHTvxFbL' : 'RAYDIUM VAULT',
  'LanMV9sAd7wArD4vJFi2qDdfnVhFxYSUg6eADduJ3uj' : 'RAYDIUM_LAUNCHLAB',  //added
  //other dexs
  'LBUZKhRxPF3XUpBCjp4YzTKgLccjZhTSDM9YuVaPwxo': 'METEORA_DLMM',
  'pAMMBay6oceH9fJKBRHGP5D4bD4sWpmSwMn52FMfXEA': 'PUMP_SWAP',
  '6EF8rrecthR5Dkzon8Nwu78hRvfCKubJ14M5uBEwF6P': 'PUMPFUN',
  'HEAVENoP2qxoeuF8Dj2oT1GHEnu49U5mJYkdeC8BAX2o': 'HEAVEN_DEX',
  '61DFfeTKM7trxYcPQCM78bJ794ddZprZpAwAnLiwTpYH': 'JUPITER_ORDER_ENGINE_PROGRAM',
  'SoLFiHG9TfgtdUXUjWAxi3LtvYuFyDLVhBWxdMZxyCe': 'SOLFI',
  'ZERor4xhbUycZ6gb9ntrhqscUcZmAbQDjEAtCf4hbZY': 'ZEROFI',
  'SwaPpA9LAaLfeLi3a68M4DjnLqgtticKg6CnyNwgAC8' : 'Token Swap', //added
  'BSwp6bEBihVLdqJRKGgzjcGLHkcTuzmSo1TQkHepzH8p' : 'Bonkswap',  //added
  'endoLNCKTqDn8gSVnN2hDdpgACUPWHZTwoYnnMybpAT' : 'Solayer',  //added
  '5ocnV1qiCgaQR8Jb8xWnVbApfaygJ8tNoZfgPwsgx9kx' : 'Sanctum Infinity',  //added
  'PSwapMdSai8tjrEXcxFeQth87xC4rRsa4VA5mhGhXkP' : 'Penguin',  //added
  'DjVE6JNiYqPL2QXyCUUh8rNjHrbz9hXHNYt99MQ59qw1' : 'Orca V1',  //added
  '9W959DqEETiGZocYWCQPaJ6sBmUzgfxXfqGeTEdp3aQP' : 'Orca V2',  //added
  'cpamdpZCGKUy5JxQXB4dcpGPiikHawvSWAd6mEn1sGG' : 'Meteora DAMM v2'  //added
};

// Token price API configuration
const PRICE_API_URL = 'https://lite-api.jup.ag/price/v3';
let lastPriceRequestTime = 0;
const MIN_PRICE_INTERVAL = 1100;

// Helius API configuration for token metadata
const HELIUS_API_URL = 'https://mainnet.helius-rpc.com/?api-key=cc7925c8-0e33-434d-9529-25b3166a1356';
let lastMetadataRequestTime = 0;
const MIN_METADATA_INTERVAL = 500; // 0.5 seconds for metadata requests
// Cache for token metadata to avoid repeated API calls
const tokenMetadataCache = new Map();

// Determine if transaction data indicates a swap (using existing classification logic)
function transactionIndicatesSwap(tx) {
  // Check if transaction has the basic structure we need
  if (!tx || !tx.transaction) return false;
  
  const { transaction, meta } = tx.transaction;
  if (!transaction || !meta) return false;
  
  // Use the same classification logic as real-time processing
  const classification = classifyTransaction(
    meta.logMessages || [],
    transaction.message?.accountKeys || []
  );
  
  return classification.type === 'swap';
}

// Compute activity summary over the last 100 txs using simplified swap detection
function summarizeRecentActivity(transactions) {
  let priorTxCount = 0;
  let priorSwapCount = 0;
  for (const tx of transactions) {
    priorTxCount += 1;
    // Check if this transaction indicates a swap
    if (transactionIndicatesSwap(tx)) {
      priorSwapCount += 1;
    }
  }
  return { priorTxCount, priorSwapCount };
}

// ============================================================================
// PROCESS 1: WHALE ADDRESS DETECTION
// ============================================================================
// ============================================================================
// PROCESS 2: BLACKLIST PROCESSING
// ============================================================================

// Function to process addresses in parallel batches
// ============================================================================
// PROCESS 3: SWAP MONITORING
// ============================================================================

// Function to update swap monitoring every 3-5 minutes
async function updateSwapMonitoring() {
  if (isSwapMonitoringRunning) return;
  isSwapMonitoringRunning = true;

  try {
    // Load current qualified addresses
    loadSwapAddresses();
    
    if (SOLANA_ACCOUNTS.length === 0) {
      console.log('No qualified addresses for monitoring');
      return;
    }

    // Resubscribe WebSocket
    if (ws && ws.readyState === WebSocket.OPEN) {
      console.log(`Updating swap monitoring for ${SOLANA_ACCOUNTS.length} addresses`);
      logToFile(`Updating swap monitoring for ${SOLANA_ACCOUNTS.length} addresses`);
      resubscribeBatch(ws);
    }

  } catch (error) {
    console.error('Error updating swap monitoring:', error.message);
    logToFile(`Error updating swap monitoring: ${error.message}`);
  } finally {
    isSwapMonitoringRunning = false;
  }
}

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================

// Function to save qualified addresses to swap_address.txt
async function saveQualifiedAddresses() {
  try {
    const addresses = Array.from(qualifiedAddresses);
    const content = `# Qualified addresses for swap monitoring\n# Generated: ${new Date().toISOString()}\n${addresses.join('\n')}\n`;
    fs.writeFileSync(SWAP_ADDRESS_FILE, content, 'utf8');
    console.log(`Saved ${addresses.length} qualified addresses to swap_address.txt`);
    logToFile(`Saved ${addresses.length} qualified addresses to swap_address.txt`);
  } catch (error) {
    console.error('Error saving qualified addresses:', error.message);
    logToFile(`Error saving qualified addresses: ${error.message}`);
  }
}

// Function to clean up duplicate addresses in swap_address.txt
function cleanupDuplicateAddresses() {
  try {
    if (!fs.existsSync(SWAP_ADDRESS_FILE)) {
      console.log('No swap_address.txt file to clean up');
      return;
    }

    const fileContent = fs.readFileSync(SWAP_ADDRESS_FILE, 'utf8');
    const addresses = fileContent
      .split('\n')
      .map(line => line.trim())
      .filter(line => line.length > 0 && !line.startsWith('#'))
      .filter(line => /^[A-Za-z0-9]{32,44}$/.test(line));

    // Remove duplicates using Set
    const uniqueAddresses = [...new Set(addresses)];
    
    if (addresses.length !== uniqueAddresses.length) {
      const duplicateCount = addresses.length - uniqueAddresses.length;
      console.log(`🧹 Found ${duplicateCount} duplicate addresses, cleaning up...`);
      logToFile(`Found ${duplicateCount} duplicate addresses, cleaning up...`);
      
      // Save cleaned addresses
      const content = `# Qualified addresses for swap monitoring\n# Generated: ${new Date().toISOString()}\n# Cleaned up ${duplicateCount} duplicates\n${uniqueAddresses.join('\n')}\n`;
      fs.writeFileSync(SWAP_ADDRESS_FILE, content, 'utf8');
      
      console.log(`✅ Cleaned up ${duplicateCount} duplicates, now ${uniqueAddresses.length} unique addresses`);
      logToFile(`Cleaned up ${duplicateCount} duplicates, now ${uniqueAddresses.length} unique addresses`);
      
      // Update SOLANA_ACCOUNTS array
      SOLANA_ACCOUNTS = uniqueAddresses;
    } else {
      console.log(`✅ No duplicates found in swap_address.txt (${addresses.length} addresses)`);
      SOLANA_ACCOUNTS = uniqueAddresses;
    }
  } catch (error) {
    console.error('Error cleaning up duplicate addresses:', error.message);
    logToFile(`Error cleaning up duplicate addresses: ${error.message}`);
  }
}

// Function to save rejected addresses to rejected_addresses.txt
async function saveRejectedAddresses() {
  try {
    const addresses = Array.from(rejectedAddresses);
    const content = `# Rejected addresses - do not re-request\n# Generated: ${new Date().toISOString()}\n${addresses.join('\n')}\n`;
    fs.writeFileSync(REJECTED_ADDRESSES_FILE, content, 'utf8');
    console.log(`Saved ${addresses.length} rejected addresses to rejected_addresses.txt`);
    logToFile(`Saved ${addresses.length} rejected addresses to rejected_addresses.txt`);
  } catch (error) {
    console.error('Error saving rejected addresses:', error.message);
    logToFile(`Error saving rejected addresses: ${error.message}`);
  }
}

// Function to load rejected addresses on startup
function loadRejectedAddresses() {
  try {
    if (fs.existsSync(REJECTED_ADDRESSES_FILE)) {
      const fileContent = fs.readFileSync(REJECTED_ADDRESSES_FILE, 'utf8');
      const addresses = fileContent
        .split('\n')
        .map(line => line.trim())
        .filter(line => line.length > 0 && !line.startsWith('#'));
      
      // Remove duplicates using Set (even though Set would do this anyway)
      const uniqueAddresses = [...new Set(addresses)];
      
      rejectedAddresses = new Set(uniqueAddresses);
      
      if (addresses.length !== uniqueAddresses.length) {
        const duplicateCount = addresses.length - uniqueAddresses.length;
        console.log(`🧹 Cleaned ${duplicateCount} duplicates from rejected_addresses.txt`);
        logToFile(`Cleaned ${duplicateCount} duplicates from rejected_addresses.txt`);
        
        // Save cleaned file
        const content = `# Rejected addresses - do not re-request\n# Generated: ${new Date().toISOString()}\n# Cleaned up ${duplicateCount} duplicates\n${uniqueAddresses.join('\n')}\n`;
        fs.writeFileSync(REJECTED_ADDRESSES_FILE, content, 'utf8');
      }
      
      console.log(`Loaded ${uniqueAddresses.length} rejected addresses from file`);
      logToFile(`Loaded ${uniqueAddresses.length} rejected addresses from file`);
    }
  } catch (error) {
    console.error('Error loading rejected addresses:', error.message);
    logToFile(`Error loading rejected addresses: ${error.message}`);
  }
}

// Function to watch for changes in whales_address.txt file
function watchWhalesAddressFile() {
  try {
    if (fs.existsSync(WHALES_ADDRESS_FILE)) {
      
      // Watch for file changes
      fs.watch(WHALES_ADDRESS_FILE, (eventType, filename) => {
        if (eventType === 'change') {
          console.log('Whales address file changed, detecting new addresses...');
          logToFile('Whales address file changed, detecting new addresses...');
          detectNewWhaleAddresses();
        }
      });
      
      console.log(`Watching for changes in: ${WHALES_ADDRESS_FILE}`);
      logToFile(`Watching for changes in: ${WHALES_ADDRESS_FILE}`);
    } else {
      console.warn(`Whales address file not found, cannot watch: ${WHALES_ADDRESS_FILE}`);
      logToFile(`Whales address file not found, cannot watch: ${WHALES_ADDRESS_FILE}`);
    }
  } catch (error) {
    console.error('Error setting up file watcher:', error.message);
    logToFile(`Error setting up file watcher: ${error.message}`);
  }
}

// Function to start the three-process system
function startThreeProcessSystem() {
  // Process 2: Blacklist processing (every 5 seconds if queue has addresses)
  setInterval(() => {
    processBlacklist();
  }, 5000);

  // Process 3: Swap monitoring update (every 4 minutes)
  setInterval(() => {
    updateSwapMonitoring();
  }, SWAP_UPDATE_INTERVAL);

  console.log('Optimized three-process system started (file-watcher based detection)');
  logToFile('Optimized three-process system started (file-watcher based detection)');
}

// Ensure log directory exists
const logDir = path.join(__dirname, 'log');
if (!fs.existsSync(logDir)) {
  fs.mkdirSync(logDir);
}

const now = new Date().toISOString().replace(/[:]/g, '-').replace('T', '_').replace('Z', '');
const logFile = path.join(logDir, `${now}.log`);

// Helper function to append logs to file with timestamp
function logToFile(message) {
  const timestamp = new Date().toISOString();
  const logEntry = `[${timestamp}] ${message}\n`;
  fs.appendFile(logFile, logEntry, (err) => {
    if (err) console.error('Failed to write log:', err);
  });
}

// Function to delay execution
function delay(ms) {
  return new Promise(resolve => setTimeout(resolve, ms));
}

// Function to fetch token price from Jupiter API
async function fetchTokenPrice(mintAddress) {
  const now = Date.now();
  const timeSinceLastRequest = now - lastPriceRequestTime;
  if (timeSinceLastRequest < MIN_PRICE_INTERVAL) {
    await delay(MIN_PRICE_INTERVAL - timeSinceLastRequest);
  }

  const url = `${PRICE_API_URL}?ids=${mintAddress}`;
  lastPriceRequestTime = Date.now();

  try {
    const response = await fetch(url);
    if (!response.ok) {
      throw new Error(`HTTP error! status: ${response.status}`);
    }
    const json = await response.json();

    if (!json[mintAddress]) {
      console.warn(`No price data found for mint address: ${mintAddress}`);
      return null;
    }

    return json[mintAddress].usdPrice;
  } catch (error) {
    console.error('Error fetching token price:', error.message);
    return null;
  }
}

// Function to send Telegram notification
async function sendTelegramNotification(message, signature = null) {
  if (!TELEGRAM_BOT_TOKEN || TELEGRAM_BOT_TOKEN === 'YOUR_BOT_TOKEN_HERE' || 
      !TELEGRAM_CHAT_ID || TELEGRAM_CHAT_ID === 'YOUR_CHAT_ID_HERE') {
    console.warn('Telegram bot token or chat ID not configured. Skipping notification.');
    return false;
  }

  const url = `https://api.telegram.org/bot${TELEGRAM_BOT_TOKEN}/sendMessage`;
  // const data = JSON.stringify({
  let messageData = {
    chat_id: TELEGRAM_CHAT_ID,
    text: message,
    parse_mode: 'HTML',
    disable_web_page_preview: true
  }
  if (signature) {
    messageData.reply_markup = {
      inline_keyboard: [
        [{
          text: "ViewTX",
          url: `https://solscan.io/tx/${signature}`
        }]
      ]
    };
  }

  const data = JSON.stringify(messageData);

  return new Promise((resolve) => {
    const options = {
      hostname: 'api.telegram.org',
      port: 443,
      path: `/bot${TELEGRAM_BOT_TOKEN}/sendMessage`,
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
        'Content-Length': data.length
      }
    };

    const req = https.request(options, (res) => {
      let responseData = '';
      res.on('data', (chunk) => {
        responseData += chunk;
      });
      res.on('end', () => {
        if (res.statusCode === 200) {
          console.log('Telegram notification sent successfully');
          logToFile('Telegram notification sent successfully');
          resolve(true);
        } else {
          console.error('Failed to send Telegram notification:', res.statusCode, responseData);
          logToFile(`Failed to send Telegram notification: ${res.statusCode} ${responseData}`);
          resolve(false);
        }
      });
    });

    req.on('error', (error) => {
      console.error('Error sending Telegram notification:', error.message);
      logToFile(`Error sending Telegram notification: ${error.message}`);
      resolve(false);
    });

    req.write(data);
    req.end();
  });
}

// Function to calculate token transfer amounts
function calculateTokenTransfers(preTokenBalances, postTokenBalances) {
  const transfers = [];
  
  // Create maps for easy lookup
  const preMap = new Map();
  const postMap = new Map();
  
  preTokenBalances.forEach(balance => {
    const key = `${balance.accountIndex}_${balance.mint}`;
    preMap.set(key, balance);
  });
  
  postTokenBalances.forEach(balance => {
    const key = `${balance.accountIndex}_${balance.mint}`;
    postMap.set(key, balance);
  });
  
  // Calculate changes for all tokens
  const allKeys = new Set([...preMap.keys(), ...postMap.keys()]);
  
  allKeys.forEach(key => {
    const pre = preMap.get(key);
    const post = postMap.get(key);
    
    if (pre && post) {
      // Token existed before and after
      const preAmount = parseFloat(pre.uiTokenAmount.uiAmountString || '0');
      const postAmount = parseFloat(post.uiTokenAmount.uiAmountString || '0');
      const change = postAmount - preAmount;
      
      if (Math.abs(change) > 0.000001) { // Significant change threshold
        transfers.push({
          mint: pre.mint,
          accountIndex: pre.accountIndex,
          owner: pre.owner,
          preAmount,
          postAmount,
          change,
          changeAmount: Math.abs(change)
        });
      }
    } else if (pre && !post) {
      // Token was completely removed
      const preAmount = parseFloat(pre.uiTokenAmount.uiAmountString || '0');
      transfers.push({
        mint: pre.mint,
        accountIndex: pre.accountIndex,
        owner: pre.owner,
        preAmount,
        postAmount: 0,
        change: -preAmount,
        changeAmount: preAmount
      });
    } else if (!pre && post) {
      // Token was newly created
      const postAmount = parseFloat(post.uiTokenAmount.uiAmountString || '0');
      transfers.push({
        mint: post.mint,
        accountIndex: post.accountIndex,
        owner: post.owner,
        preAmount: 0,
        postAmount,
        change: postAmount,
        changeAmount: postAmount
      });
    }
  });
  
  // Sort by absolute change amount (biggest changes first)
  transfers.sort((a, b) => b.changeAmount - a.changeAmount);
  
  return transfers;
}

// Function to classify transaction type and identify DEX
function classifyTransaction(logMessages, accountKeys) {
  const dexAddresses = new Set();
  const hasJupiter = logMessages.some(log => log.includes('JUP'));
  const hasOKX = logMessages.some(log => log.includes('okx'));
  
  // Extract DEX addresses from log messages and account keys
  logMessages.forEach(log => {
    Object.keys(DEX_ADDRESSES).forEach(address => {
      if (log.includes(address)) {
        dexAddresses.add(DEX_ADDRESSES[address]);
      }
    });
  });
  
  // Check account keys for DEX addresses
  accountKeys.forEach(account => {
    if (DEX_ADDRESSES[account.pubkey]) {
      dexAddresses.add(DEX_ADDRESSES[account.pubkey]);
    }
  });
  
  const dexList = Array.from(dexAddresses);
  
  // Classification logic
  if (dexList.length === 0) {
    return {
      type: 'transfer',
      dex: null,
      reason: 'No DEX addresses found'
    };
  }
  
  if (hasJupiter && hasOKX && dexList.length > 2) {
    return {
      type: 'skip',
      dex: null,
      reason: 'Jupiter and OKX aggregator with over 2 DEX addresses - ignoring'
    };
  }
  
  if ((hasJupiter || hasOKX) && dexList.length === 1) {
    return {
      type: 'swap',
      dex: dexList[0],
      reason: `${hasJupiter ? 'Jupiter' : 'OKX'} aggregator with single DEX: ${dexList[0]}`
    };
  }
  
  if (dexList.length === 1) {
    return {
      type: 'swap',
      dex: dexList[0],
      reason: `Single DEX found: ${dexList[0]}`
    };
  }
  
  if (dexList.length > 1) {
    return {
      type: 'swap',
      dex: dexList.join(', '),
      reason: `Multiple DEX addresses found: ${dexList.join(', ')}`
    };
  }
  
  return {
    type: 'unknown',
    dex: null,
    reason: 'Unable to classify transaction'
  };
}

// Function to process transaction and send notification
async function processTransactionAndNotify(transactionData, triggeringAddress) {
  try {
    const { transaction, meta } = transactionData.transaction;
    
    if (!meta || !transaction) {
      return null;
    }
    
    // Extract basic transaction info
    const signature = transaction.signatures?.[0] || 'Unknown';
    const slot = transactionData.slot || 'Unknown';
    const fee = meta.fee || 0;
    const success = meta.err === null;

    // DEBUG: Show current monitoring addresses
    console.log(`📍 Current SOLANA_ACCOUNTS: ${SOLANA_ACCOUNTS.length} addresses`);
    console.log(`📍 First few: ${SOLANA_ACCOUNTS.slice(0, 3).join(', ')}`);
    
    // DEBUG: Show token balances
    console.log(`📍 Pre-token balances: ${(meta.preTokenBalances || []).length}`);
    console.log(`📍 Post-token balances: ${(meta.postTokenBalances || []).length}`);
    
    // Calculate token transfers
    const tokenTransfers = calculateTokenTransfers(
      meta.preTokenBalances || [],
      meta.postTokenBalances || []
    );
    
    // Classify transaction
    const classification = classifyTransaction(
      meta.logMessages || [],
      transaction.message?.accountKeys || []
    );
    
    // Find the biggest token change
    const biggestChange = tokenTransfers.length > 0 ? tokenTransfers[0] : null;
    
    // Extract account keys for analysis
    const accountKeys = transaction.message?.accountKeys || [];
    const accountAddresses = accountKeys.map(acc => acc.pubkey);

    const triggeredAddress = triggeringAddress;
    
    const result = {
      signature,
      slot,
      fee,
      success,
      type: classification.type,
      dex: classification.dex,
      reason: classification.reason,
      tokenTransfers,
      biggestChange,
      accountAddresses,
      triggeredAddress,
      logMessages: meta.logMessages || [],
      preTokenBalances: meta.preTokenBalances || [],
      postTokenBalances: meta.postTokenBalances || []
    };

    if (classification.type === 'swap' && biggestChange && Math.abs(biggestChange.change) > 0.000001) {
      console.log(`🚨 SENDING SWAP NOTIFICATION: ${signature} (${classification.dex})`);
      await sendSwapNotification(result);
    } else {
      console.log(`❌ NOT SENDING: type=${classification.type}, hasTokenChange=${biggestChange ? Math.abs(biggestChange.change) : 0}`);
    }
    
    return result;
    
  } catch (error) {
    console.error('Error processing transaction:', error);
    return null;
  }
}

// Function to send swap notification to Telegram
async function sendSwapNotification(processedTx) {
  try {
    const { signature, dex, biggestChange, triggeredAddress} = processedTx;
    
    if (!biggestChange || !biggestChange.mint) {
      console.log('No token change detected, sending generic transaction notification');
      
      // Send a generic notification for the transaction
      let message = `🔔 Transaction Alert\n\n`;
      message += `📝 Type: ${processedTx.type}\n`;
      message += `💰 DEX: ${dex || 'Unknown'}\n`;
      message += `👤 Address: <code>${triggeredAddress}</code>\n`;
      message += `🔗 Signature: <code>${signature}</code>\n`;
      
      if (config.NOTIFICATION_SETTINGS.INCLUDE_SIGNATURE) {
        message += `\n<a href="https://solscan.io/tx/${signature}">View Transaction</a>`;
      }
      
      await sendTelegramNotification(message, signature);
      console.log('Generic transaction notification sent');
      return;
    }

    // Determine if it's a buy or sell
    const isBuy = biggestChange.change < 0;
    const swapType = isBuy ? config.NOTIFICATION_SETTINGS.BUY_EMOJI : config.NOTIFICATION_SETTINGS.SELL_EMOJI;
    const action = isBuy ? 'Bought' : 'Sold';
    
    // Get token price and metadata
    const [tokenPrice, tokenMetadata] = await Promise.all([
      fetchTokenPrice(biggestChange.mint),
      fetchTokenMetadata(biggestChange.mint)
    ]);
    
    if (tokenPrice === null) {
      console.log(`Could not fetch price for token ${biggestChange.mint}, skipping notification`);
      return;
    }
    
    // Calculate total USD value
    const totalUSD = tokenPrice * Math.abs(biggestChange.change);
    
    // Check minimum USD value threshold
    if (totalUSD < config.NOTIFICATION_SETTINGS.MIN_USD_VALUE) {
      console.log(`Swap value $${totalUSD.toFixed(2)} below threshold $${config.NOTIFICATION_SETTINGS.MIN_USD_VALUE}, skipping notification`);
      return;
    }
    
    // Format the message
    const tokenPriceDisplay = tokenPrice ? `$${tokenPrice.toFixed(6)}` : 'N/A';
    
    let message = `${swapType} Swap on #${dex.toLowerCase()}\n\n`;
    message += `👉 token: ${tokenMetadata ? `${tokenMetadata.symbol}(${tokenMetadata.name})` : biggestChange.mint.slice(0, 8) + '...'}\n`;
    message += `💰 token amount: ${Math.abs(biggestChange.change).toFixed(6)}\n`;
    message += `💵 usd value: $${totalUSD.toFixed(2)}\n`;
    message += `📈 token price: ${tokenPriceDisplay}\n`;
    message += `token mint: <code style="color: #4709bbff; font-family: monospace;">${biggestChange.mint}</code>\n`;
    message += `trader address: <code style="color: #4709bbff; font-family: monospace;">${triggeredAddress}</code>\n`;
 
    
    if (config.NOTIFICATION_SETTINGS.INCLUDE_SIGNATURE) {
      message += `\n\n<a href="https://solscan.io/tx/${signature}">ViewTx</a>`;
    }
    
    // Send Telegram notification
    await sendTelegramNotification(message, signature);
    
    console.log('Swap notification sent:', message);
    logToFile(`Swap notification sent: ${message}`);
    
  } catch (error) {
    console.error('Error sending swap notification:', error);
    logToFile(`Error sending swap notification: ${error.message}`);
  }
}


// Initialize WebSocket connection
let ws;
let reconnectAttempts = 0;
let reconnectTimer = null

function initWebSocket() {
  ws = new WebSocket(WS_URL);

  ws.on('open', async () => {
    reconnectAttempts = 0;
    const msg = 'WebSocket is open';
    console.log(msg);
    logToFile(msg);

    // Load addresses for monitoring from swap_address.txt
    console.log('🔄 Loading addresses from swap_address.txt for monitoring...');
    loadSwapAddresses();
    
    if (SOLANA_ACCOUNTS.length === 0) {
      console.log('⚠️ No addresses loaded from swap_address.txt, checking whales_address.txt...');
      // detectWhaleAddresses will fill queues; monitoring will update on next cycle
      detectWhaleAddresses();
    } else {
      console.log(`✅ Loaded ${SOLANA_ACCOUNTS.length} addresses for monitoring`);
    }

    if (SOLANA_ACCOUNTS.length > 0) {
      console.log('📡 Subscribing to WebSocket for transaction monitoring...');
      resubscribeBatch(ws);
    } else {
      console.log('❌ No addresses to monitor - WebSocket subscription skipped');
    }
    
    startPing(ws);
  });
}
// Helper to unsubscribe the existing batch subscription (if any) and resubscribe
function resubscribeBatch(ws) {
  if (currentBatchSubscriptionId) {
    try {
      const unsubscribeRequest = {
        jsonrpc: "2.0",
        id: 501,
        method: "transactionUnsubscribe",
        params: [currentBatchSubscriptionId]
      };
      ws.send(JSON.stringify(unsubscribeRequest));
      console.log(`🔕 Unsubscribed previous batch with ID: ${currentBatchSubscriptionId}`);
      logToFile(`Unsubscribed previous batch with ID: ${currentBatchSubscriptionId}`);
    } catch (e) {
      console.error('Error unsubscribing previous batch:', e.message);
      logToFile(`Error unsubscribing previous batch: ${e.message}`);
    }
    currentBatchSubscriptionId = null;
  }
  sendBatchSubscriptionRequest(ws);
}

// Function to send subscription requests for all monitored addresses
function sendAllRequests(ws) {
  if (SOLANA_ACCOUNTS.length === 0) {
    console.log('❌ No addresses in SOLANA_ACCOUNTS - skipping WebSocket subscription');
    return;
  }

  console.log(`📡 Sending batch WebSocket subscription for ${SOLANA_ACCOUNTS.length} addresses`);
  sendBatchSubscriptionRequest(ws);
}

// Function to send ping frames every 30 seconds to keep the connection alive
function startPing(ws) {
  const interval = setInterval(() => {
    if (ws.readyState === WebSocket.OPEN) {
      ws.ping();
      const msg = 'Ping sent';
      console.log(msg);
      logToFile(msg);
    } else {
      clearInterval(interval);
      const msg = 'Ping interval cleared as WebSocket is no longer open';
      console.log(msg);
      logToFile(msg);
    }
  }, 30000);
}

// WebSocket event handlers
ws.on('open', async () => {
  const msg = 'WebSocket is open';
  console.log(msg);
  logToFile(msg);

  console.log('🔄 Loading addresses from swap_address.txt for monitoring...');
  const loaded = loadSwapAddresses();

  if (SOLANA_ACCOUNTS.length === 0) {
    console.log('⚠️ No addresses loaded from swap_address.txt, checking whales_address.txt...');
    await loadWhalesAddresses(); // This will trigger enrollment if needed
  } else {
    console.log(`✅ Loaded ${SOLANA_ACCOUNTS.length} addresses for monitoring`);
  }
  
  if (SOLANA_ACCOUNTS.length > 0) {
    console.log('📡 Subscribing to WebSocket for transaction monitoring...');
    sendAllRequests(ws);
  } else {
    console.log('❌ No addresses to monitor - WebSocket subscription skipped');
  }

  startPing(ws);
});

// Export functions for testing or external use
module.exports = {
  calculateTokenTransfers,
  classifyTransaction,
  processTransactionAndNotify,
  sendTelegramNotification,
  fetchTokenPrice,
  fetchTokenMetadata,
  sendSwapNotification,
  DEX_ADDRESSES,
  saveSwapAddresses,
  loadSwapAddresses,
  cleanupDuplicateAddresses,
  detectWhaleAddresses,
  detectNewWhaleAddresses,
  processBlacklist,
  updateSwapMonitoring,
  watchWhalesAddressFile,
  watchSwapAddressFile,
  startThreeProcessSystem
};
