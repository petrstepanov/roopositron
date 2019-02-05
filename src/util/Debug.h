/*
 * Debug.h
 *
 *  Created on: Feb 4, 2019
 *      Author: student
 */

#ifndef SRC_UTIL_DEBUG_H_
#define SRC_UTIL_DEBUG_H_

// http://www.cplusplus.com/forum/beginner/42190/

#define USEDEBUG

#ifdef USEDEBUG
#define Debug( x ) std::cout << std::endl << x << std::endl
#else
#define Debug( x )
#endif

#endif /* SRC_UTIL_DEBUG_H_ */
