//
//  Prog3Settings.h
//  Prog3
//
//  Created by Knuth Lohse on 30.05.13.
//  Copyright (c) 2013 Knuth Lohse. All rights reserved.
//

#ifndef _PROG3_PROG3SETTINGS_H_
#define _PROG3_PROG3SETTINGS_H_

//#define _DEBUG_
//#define _USE_HARDCODED_TASKCONFIG_

#define _USE_BOOST_REGEX_
//#define _USE_STD_REGEX_

#ifdef _USE_BOOST_REGEX_
    #define _REGEX_PREFIX_ boost
#endif //_USE_BOOST_REGEX_

#ifdef _USE_STD_REGEX_
    #define _REGEX_PREFIX_ std
#endif //_USE_STD_REGEX_

#endif //_PROG3_PROG3SETTINGS_H_

