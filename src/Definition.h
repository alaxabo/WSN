/*
 * Definition.h
 *
 *  Created on: Nov 15, 2014
 *      Author: LTT
 */

#ifndef DEFINITION_H_
#define DEFINITION_H_

//energy
#define Eelec 50 //nJ//0.00005 //
#define Efs 0.1 //100pJ//
#define Emp 0.0000013 //nJ
#define Eda 5 //nJ data aggregation

//Message Length
#define HeadMsgLength 100 //bit = 25bytes
#define DataMsgLength 500 //bit = 500bytes

//node status

//message types
#define SELF_SMSG_INIT 11
#define SMSG_INIT 12
//#define ENERGY_REQUEST 13
//#define ENERGY_CONFIRM 14
//#define CLUSTER_INFO 15
#define TDMA_INIT 13
#define SELF_DATA_TO_CH 14
#define DATA_TO_CH 15
#define DATA_TO_BS 16

#define ROUND_TO_RECLUSTER 256

#endif /* DEFINITION_H_ */
