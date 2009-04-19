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

#define COOKIE_SDMX       0x53444d58    // 'SDMX'
#define COOKIE_SDMX_ENID  0x456e4964    // 'EnId'
#define COOKIE_SDMX_UNAM  0x554e616d    // 'UNam'
#define COOKIE_SDMX_CHBK  0x4368426b    // 'ChBk'

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
  uint8             ChannelLevels[];   // Raw channel levels. 
}; 

// Set External Source message
struct CITP_SDMX_SXSr
{ 
  CITP_SDMX_Header  CITPSDMXHeader;       // CITP SDMX header. SDMX ContentType is "SXSr". 
  //ucs1              ConnectionString[];   // DMX-source connection string. See DMX 
                                          // Connection Strings in Definitions. 
};


#endif // _CITPDEFINES_H_
