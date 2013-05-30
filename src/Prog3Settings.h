//
//  Prog3Settings.h
//  Prog3
//
//  Created by Knuth Lohse on 30.05.13.
//  Copyright (c) 2013 Knuth Lohse. All rights reserved.
//

#ifndef Prog3_Prog3Settings_h
#define Prog3_Prog3Settings_h

#define _DEBUG_
#define _USE_HARDCODED_TASKCONFIG_

//#define _USE_BOOST_REGEX_
#define _USE_STD_REGEX_

#ifdef _USE_BOOST_REGEX_
    #define _REGEX_PREFIX_ boost
#endif

#ifdef _USE_STD_REGEX_
    #define _REGEX_PREFIX_ std
#endif

#endif
