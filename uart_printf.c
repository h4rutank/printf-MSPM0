/*
//@overview
//Impliment kind of printf() on MSPM0
//
//@usage
//Print %c, %s, and %d
//
//@license MIT License
//Copyright (c) 2025 h4rutank
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.
*/

#include <uart_printf.h>
#include "ti_msp_dl_config.h"

#ifdef __cplusplus
	#include <cstdint>
#else
	#include <stdint.h>
	#include <stdbool.h>
#endif


//To Redirect to UART output
void putchar_(char c)
{
    /* Wait until TX FIFO is transmitted and the TX FIFO is not full */
    while (DL_UART_isTXFIFOFull(UART_0_INST));
    DL_UART_transmitData(UART_0_INST, c);
}

//Print character
static void print_char(char c)
{
	putchar_(c);
}

//Print string
static void print_string(const char *s)
{
	while(*s) putchar_(*s++);
}

//Print integral value
static void print_int(int val)
{
	char buf[16];
	int i = 0;
	bool neg_sign = false;

	//Inverse value if it is negative
	if(val<0){
		neg_sign = true;
		val = -val;
	}

	//Convert value to string
	do{
		buf[i++] = '0' + (val%10);
		val /= 10;
	}while(val > 0 && i < sizeof(buf));

	if(neg_sign) buf[i++] = '-';

	while(i--) putchar_(buf[i]);
}

//Implement printf_
void printf_(const char *fmt, ...)
{
	va_list args;	//Variable Length Arguments
	va_start(args, fmt);	//Read Variable Length Arguments

	while(*fmt){
		if(*fmt == '%'){
			fmt++;
			switch(*fmt){
			case 'd':
			{
				int v = va_arg(args, int);	//Read int variable
				print_int(v);
				break;
			}
			case 's':
			{
				char *s = va_arg(args, char*);	//Read char* variable
				print_string(s);
				break;
			}
			case 'c':
			{
				char c = (char) va_arg(args, char);	//Read char variable
				print_char(c);
				break;
			}
			default:
				putchar_('%');
				break;
			}
		}
		else{
			putchar_(*fmt);
		}
		fmt++;
	}

	va_end(args);
}
