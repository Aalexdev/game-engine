/**
 * MIT License
 * 
 * Copyright (c) 2022 Alexis Magnier
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _RAINDROP_HPP_
#define _RAINDROP_HPP_

#ifdef RD_BUILD_DYNAMIC
	#if defined(__WIN32__) || defined(__WINRT__) || defined(__CYGWIN__) || defined(__OS2__)
		#ifdef RD_BUILD
			#define RD_API __declspec(dllexport)
		#else
			#define RD_API __declspec(dllimport)
		#endif
	#else
		#define RD_API
	#endif
#else
	#define RD_API
#endif

namespace RainDrop{
	void RD_API initialize();
}

#endif // _RAINDROP_HPP_