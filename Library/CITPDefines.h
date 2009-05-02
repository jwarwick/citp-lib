// -*- mode: C++ -*-
#ifndef _CITPDEFINES_H_
#define _CITPDEFINES_H_

//
// The CITP data structures and constants
//

//
// Datatypes
//

typedef qint8   int8;
typedef qint16  int16;
typedef qint32  int32;
typedef qint64  int64;

typedef quint8  uint8;
typedef quint16 uint16;
typedef quint32 uint32;
typedef quint64 unit64;

typedef unsigned char ucs1;


// 
// Content Type cookies
//

//#define COOKIE_CITP       0x43495450    // 'CITP'
#define COOKIE_CITP       0x50544943    // 'CITP'

//#define COOKIE_PINF       0x50494e46    // 'PINF'
#define COOKIE_PINF       0x464e4950    // 'PINF'
#define COOKIE_PINF_PNAM  0x504e616d    // 'PNam'
//#define COOKIE_PINF_PLOC  0x504c6f63    // 'PLoc'
#define COOKIE_PINF_PLOC  0x636f4c50    // 'PLoc'

//#define COOKIE_SDMX       0x53444d58    // 'SDMX'
#define COOKIE_SDMX       0x584d4453    // 'SDMX'
#define COOKIE_SDMX_ENID  0x456e4964    // 'EnId'
//#define COOKIE_SDMX_UNAM  0x554e616d    // 'UNam'
#define COOKIE_SDMX_UNAM  0x6d614e55    // 'UNam'
//#define COOKIE_SDMX_CHBK  0x4368426b    // 'ChBk'
#define COOKIE_SDMX_CHBK  0x6b426843    // 'ChBk'


//#define COOKIE_FPTC  0x46505443 // 'FTPC'
#define COOKIE_FPTC  0x43545046 // 'FPTC'
//#define COOKIE_FPTC_PTCH 0x50746368 // 'Ptch'
#define COOKIE_FPTC_PTCH 0x68637450 // 'Ptch'
//#define COOKIE_FPTC_UPTC 0x55507463 // 'UPtc'
#define COOKIE_FPTC_UPTC 0x63745055 // 'UPtc'
//#define COOKIE_FPTC_SPTC 0x53507463 // 'SPtc'
#define COOKIE_FPTC_SPTC 0x63745053 // 'SPtc'

//#define COOKIE_FSEL 0x4653454c // 'FSEL'
#define COOKIE_FSEL 0x4c455346 // 'FSEL'
//#define COOKIE_FSEL_SELE 0x53656c65 // 'Sele'
#define COOKIE_FSEL_SELE 0x656c6553 // 'Sele'
//#define COOKIE_FSEL_DESE 0x44655365 // 'DeSe'
#define COOKIE_FSEL_DESE 0x65536544 // 'DeSe'

//#define COOKIE_FINF       0x46494e46 // 'FINF'
#define COOKIE_FINF       0x464e4946 // 'FINF'
//#define COOKIE_FINF_SFRA  0x53467261 // 'SFra'
#define COOKIE_FINF_SFRA  0x61724653 // 'SFra'
//#define COOKIE_FINF_FRAM  0x4672616d // 'Fram'
#define COOKIE_FINF_FRAM  0x6d617246 // 'Fram'
//#define COOKIE_FINF_LSTA  0x4c537461 // 'LSta'
#define COOKIE_FINF_LSTA  0x6174534c // 'LSta'

#pragma pack(1)

//
// CITP, Base Layer
//
struct CITP_Header 
{ 
  uint32      Cookie;                        // Set to "CITP".      
  uint8       VersionMajor;                  // Set to 1. 
  uint8       VersionMinor;                  // Set to 0.       
  uint8       Reserved[2];                   // 4-byte alignment       
  uint32      MessageSize;                   // The size of the entire message, including this header.      
  uint16      MessagePartCount;              // Number of message fragments.      
  uint16      MessagePart;                   // Index of this message fragment (0-based).      
  uint32      ContentType;                   // Cookie identifying the type of contents (the name of the second layer). 
}; 


//
// CITP/PINF, Peer Information Layer
//

#define CITP_PINF_MULTICAST_IP   "224.0.0.180"
#define CITP_PINF_MULTICAST_PORT 4809

struct CITP_PINF_Header 
{ 
  CITP_Header CITPHeader;    // The CITP header. CITP ContentType is "PINF". 
  uint32      ContentType;   // A cookie defining which PINF message it is. 
}; 

// DEPRECATED, USE PLoc instead
// Peer Name message
struct CITP_PINF_PNam 
{ 
  CITP_PINF_Header  CITPPINFHeader;   // The CITP PINF header. PINF ContentType is "PNam".          
  //ucs1              Name[];           // The display name of the peer (null terminated). 
                                      // This could be anything from a 
				      // user defined alias for the peer of the name of the 
                                      // product, or a combination. 
}; 

// Peer Location message
struct CITP_PINF_PLoc 
{ 
  CITP_PINF_Header  CITPPINFHeader;     // The CITP PINF header. PINF ContentType is "PLoc". 
  uint16            ListeningTCPPort;   // The port on which the peer is listening for 
                                        // incoming TCP connections. 0 if not listening. 
  //ucs1              Type[];             // Can be "LightingConsole", "MediaServer", 
                                        // "Visualizer" or "OperationHub". 
  //ucs1              Name[];             // The display name of the peer. Corresponds to the 
                                        //     PINF/PNam/Name field. 
  //ucs1              State[];            // The display state of the peer. This can be any 
                                        //     descriptive string presentable to the user such 
                                        //     as "Idle", "Running" etc. 
};


//
// CITP, SDMX - Send DMX Layer
//

struct CITP_SDMX_Header 
{ 
  CITP_Header CITPHeader;    // CITP header. CITP ContentType is "SDMX". 
  uint32      ContentType;   // Cookie defining which SDMX message it is. 
}; 

// Encryption Identifier message
struct CITP_SDMX_EnId 
{ 
  CITP_SDMX_Header  CITPSDMXHeader;    // CITP SDMX header. SDMX ContentType is "EnId". 
  //ucs1              Identifier[];      // Encryption scheme identifier. 
}; 

// Universe Name message
struct CITP_SDMX_UNam 
{ 
  CITP_SDMX_Header  CITPSDMXHeader;    // CITP SDMX header. SDMX ContentType is "UNam". 
  uint8             UniverseIndex;     // 0-based index of the universe. 
  //ucs1              UniverseName[];    // Name of the universe. 
}; 

// Channel Block message
struct CITP_SDMX_ChBk 
{ 
  CITP_SDMX_Header  CITPSDMXHeader;    // CITP SDMX header. SDMX ContentType is "ChBk". 
  uint8             Blind;             // Set to 1 for blind preview dmx, 0 otherwise. 
  uint8             UniverseIndex;     // 0-based index of the universe. 
  uint16            FirstChannel;      // 0-based index of first channel in the universe. 
  uint16            ChannelCount;      // Number of channels. 
  //uint8             ChannelLevels[];   // Raw channel levels. 
}; 

// Set External Source message
struct CITP_SDMX_SXSr
{ 
  CITP_SDMX_Header  CITPSDMXHeader;       // CITP SDMX header. SDMX ContentType is "SXSr". 
  //ucs1              ConnectionString[];   // DMX-source connection string. See DMX 
                                          // Connection Strings in Definitions. 
};


//
// CITP, FPTC - Fixture Patch Layer
//

struct CITP_FPTC_Header 
{ 
  CITP_Header CITPHeader;    // The CITP header. CITP ContentType is "FPTC". 
  uint32      ContentType;   // A cookie defining which FSEL message it is. 
  uint32      ContentHint;   // Content hint flags. 
                             // 0x00000001    Message part of a sequence of messages. 
                             // 0x00000002    Message part of and ends a sequence of messages. 
}; 

// Fixture Patch message
struct CITP_FPTC_Ptch 
{ 
  CITP_FPTC_Header  CITPFPTCHeader;      // The CITP FPTC header. FPTC ContentType is "Ptch". 
  uint16            FixtureIdentifier;   // Fixture identifier. 
  uint8             Universe;            // Patch universe (0-based). 
  uint8             Reserved[1];         // 4-byte alignment. 
  uint16            Channel;             // Patch channel (0-based). 
  uint16            ChannelCount;        // Patch channel count (1-512). 
  //ucs1              FixtureMake[];       // Fixture make (only null if omitted). 
  //ucs1              FixtureName[];       // Fixture name (never omitted). 
}; 

// Fixture Unpatch message
struct CITP_FPTC_UPtc 
{ 
  CITP_FPTC_Header  CITPFPTCHeader;          // The CITP FPTC header. FPTC ContentType is "UPtc". 
  uint16            FixtureCount;            // Fixture count (0 to unpatch all). 
  //uint16            FixtureIdentifiers[];    // Fixture identifiers 
}; 

// Fixture Send Patch message
struct CITP_FPTC_SPtc 
{ 
  CITP_FPTC_Header  CITPFPTCHeader;          // The CITP FPTC header. FPTC ContentType is "SPtc". 
  uint16            FixtureCount;            // Fixture count (0 to request all). 
  //uint16            FixtureIdentifiers[];    // Fixture identifiers. 
};

#endif // _CITPDEFINES_H_
