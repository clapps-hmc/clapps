/*
 * mux_8x1.cpp
 *
 *  Created on: 24 de out de 2016
 *      Author: root
 */

#include "mux_8x1.h"

void mux_8x1::prc_mux_8x1(){
	unsigned select = sel.read().to_uint();
	switch(select){
	case 0:
		out.write(in[0]);
		break;
	case 1:
		out.write(in[1]);
		break;
	case 2:
		out.write(in[2]);
		break;
	case 3:
		out.write(in[3]);
		break;
	case 4:
		out.write(in[4]);
		break;
	case 5:
		out.write(in[5]);
		break;
	case 6:
		out.write(in[6]);
		break;
	case 7:
		out.write(in[7]);
		break;
	}
}
