#include "include.h"
/*

                                     Jcccc,        ,d$$$b,
                                     J$$$$$$c     ,d$$$$$$,
   spooky ghost will fuck your       $$$$$$$$$c,,c$$$$P$$$$,
   shit up if you c&p into some      J$$$$$$$$$$$$$$$$$$3$$$$,
   shitty p2c garbage               $$$$$$$$$$$$$$$$$$$F$$$$$$c=
                                   J$$$$$$$$$$$$$?$$$$$F  """
                                zcd$$$$$$$$$$$$$Fb3$$$"
                             ,c=cc$$$$$$$$$$??$$$c
                          ,z$"   `$$$$$$$$$??$.?$$b
                         d$$",d$$c$$$$$$P"  ?$Fd$$$$
                       ,$$$,dP" "$$$$$$$$$$c `$$$$$$r
                      z$$$$P"=$c $$$$$$P""?$, $$$$$$$
             .,,ccc,,4$$$$$'  `$ $$$$"-cc  $$,$$$$$$$
         ,c$$$$$$$$$$$$$$$$ ,,d$,$$P   $$FJ$$$$$$$E3F
        ,$$$$$$$$$$$$$$$$$$L"$$'$$$ ,zd$$ $$$$$$$$???-
        $$$$$$$$$$$$$$$$P""??cc$$$$ $$$F,$$$$$$$$$$$$$$$$ccc,.
        $$$$$$$$$$$$$$P d$$$$bc3$$$bc,cd$$$$$$$$$$$$$$$$$$$$$$$bc.
         $$$$$$$$$$$$$F:$$$$$$$$$P?$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$b.
         `$$$$$$$$$$$$b.?$$$$$$$$$c"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$c
           ?$$$$$$$$$$$ c,?$$$$$$$",$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$h
            `$$$$$$$$$"d$$$cc,,,,c $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$F
             `?$$$$$$$z$$$$$$$$$$$ $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$F
               ?$$$$$$$$$$$$$$$$$$ $$$$$$$$$$$$$$$$$$$$$?$$$$$$$$$P"
                $$$$$$$$$?$$$$$$$'d$$$$$$$$$$$$$$$$F',ccccccc,"''
                `$$$$$$$F "$$P$$P,$$$$$$$$$$$$$$P",$$$$$$$$$$$$$bc.
                 $$$$$P$$$$$$ $$b$$$$$$$$$$$$P?",$$$$$$$$$$$$$$$$$$$c
     .,,,,.      `$$$$$,"???",$$$$$$$$$$$$CCh$'J$$$$$$$$$$$$$$$$$$$$$$b.
 ,c$$$$$$$$$$c, $ $$$$$$$$$$$$$$$$$$$$$$$$$$$'d$$$$$$$$$$$$$$$$$$$$$$$$$$c
 $$$$$$$$$$$$$$ $ ?$$$$$$$$$$$$$$$$$$$$$$$$$'c$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 $$$$$$$$$$$$$$ $L`$$$$$$$$$$$$$$$$$$$$$$$$$,$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 $$$$$$$$$$$$$$ $$,`$$$$$$$$Fl$$$$$$$$$$$$$L$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 $$$$$$$$$$$$$$ $$$c,?$$$P"l$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 $$$$$$$$$$$$$$,`$$$$$cccd$$$$$?$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 $$$$$$$$$$$$$$$ ?$$$$$$$$$$$",$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 $$$$$$)$$$$$$$$$,"$$$$$$P",c$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 $$$$$FJ$$$$$$$$$$b,,..,,d$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/

using std::cout;
using std::endl;

BSPMap* g_pBSP = new BSPMap;

BSPMap::~BSPMap()
{}

void BSPMap::unload()
{
	delete[]   m_data;

	*m_path    = NULL;
	*m_mapName = NULL;
	m_header   = NULL;
	m_plane    = NULL;
	m_node     = NULL;
	m_leaf     = NULL;
}

bool BSPMap::IsNull()
{
	if( m_data == NULL )
		return true;
	if( *m_path == NULL )
		return true;
	if( *m_mapName == NULL )
		return true;
	if( m_header == NULL )
		return true;
	if( m_plane == NULL )
		return true;
	if( m_node == NULL )
		return true;
	if( m_leaf == NULL )
		return true;

	return false;
}



bool BSPMap::load( const char* path, const char* mapName )
{
	strcpy( m_path, path );
	strcpy( m_mapName, mapName );

	std::string fPath( m_path );
	fPath += "\\csgo\\maps\\";
	fPath += m_mapName;
	// shit man you really want this shit?
	HANDLE hFile = CreateFile( fPath.c_str(), GENERIC_READ, NULL, NULL, OPEN_ALWAYS, NULL, NULL );
	if( hFile == NULL )
	{
		return false;
	}

	DWORD dwSize = GetFileSize( hFile, NULL );
	if( !dwSize )
	{
		CloseHandle( hFile );
		return false;
	}

	m_data = new BYTE[ dwSize ];

	DWORD dwRead = NULL;
	if( !ReadFile( hFile, m_data, dwSize, &dwRead, NULL ) )
	{
		CloseHandle( hFile );
		delete[] m_data;
		return false;
	}

	// lol jk don't include this, trust me on this you don't need to close the handle
	CloseHandle( hFile );

	m_header	= ( dheader_t* )m_data;

	m_node		= ( dnode_t*  )( m_data + m_header->lumps[ LUMP_NODES ].fileofs );
	m_plane		= ( dplane_t* )( m_data + m_header->lumps[ LUMP_PLANES ].fileofs );
	m_leaf		= ( dleaf_t*  )( m_data + m_header->lumps[ LUMP_LEAVES ].fileofs );

	return true;
}

void BSPMap::DisplayInfo()
{
	if( !g_pEngine->IsInGame() )
	{
		cout << "Please enter a game first!" << endl;
	}
	else
	{
		cout << "map version  : " << m_header->version << endl;
		cout << "map name     : " << m_mapName << endl;
		cout << "map Revision : " << m_header->mapRevision << endl;
		cout << endl << endl;
	}
}

void BSPMap::setPath( const char* path )
{
	strcpy( m_path, path );
}

int BSPMap::getVersion()
{
	return m_header->version;
}

int BSPMap::getRevision()
{
	return m_header->mapRevision;
}

const char* BSPMap::getPath()
{
	return m_path;
}

const char* BSPMap::getName()
{
	return m_mapName;
}

dnode_t* BSPMap::getNodeLump()
{
	return m_node;
}

dleaf_t* BSPMap::getLeafLump()
{
	return m_leaf;
}

dplane_t* BSPMap::getPlaneLump()
{
	return m_plane;
}

dleaf_t* BSPMap::GetLeafFromPoint( const Vector3 point )
{
	int nodenum = 0;
	dnode_t* node;
	dplane_t* plane;

	float d = 0.0f;

	while( nodenum >= 0 )
	{
		if( &m_node == NULL || &m_plane == NULL )
			return NULL;

		node = &m_node[ nodenum ];
		plane = &m_plane[ node->planenum ];
		d = g_pMath->Dot( point, plane->normal ) - plane->dist;
		if( d > 0 )
			nodenum = node->children[ 0 ];
		else
			nodenum = node->children[ 1 ];
	}

	return &m_leaf[ -nodenum - 1 ];
}

bool BSPMap::Visible( const Vector3 &vStart, const Vector3 &vEnd ) // added in const so it won't try to refresh the same value
{
	if( IsNull() )
		return false;

	Vector3 direction = vEnd - vStart;
	Vector3 point = vStart;

	int steps = ( int )g_pMath->VecLength( direction );

	if( steps > 4000 )	// performence issue when checking long distances, 2000 too short 
		return false;   // we'll assume we can't see someone at great lengths
	
	direction /= ( float )steps;

	dleaf_t* leaf = nullptr;

	while( steps )
	{
		point += direction;
		leaf = GetLeafFromPoint( point );
		// Tried differenent masks, none seem to work :/ becauce ur a bitch tbh tbf fam
		if( leaf->contents & CONTENTS_SOLID )	
			return false;

		--steps;
	}
	return true;
}