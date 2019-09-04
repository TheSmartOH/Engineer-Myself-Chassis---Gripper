#include "Func_Usermath.h"
#include "string.h"


/*
 ******************************************************************************************************************************
 *	名    字：AvgFilter()
 *	参    数：s16 *param,u8 length 
 *	返回值  ：s16
 *	功能描述：去掉最大值，最小值求平均值
 *	建立时间：2016.10.06  茹凯琪
 *	备    注：每次修改时请标注修改理由，时间，姓名
 *			根据所给参数中的长度来判断求平均值，其长度可能不是固定的
 ******************************************************************************************************************************
*/
s16 AvgFilter(s16 *param,u8 length)
{	
	if(length > 3){//当数组中的数字太少时不用去掉最值
		s16 max,min;
		int32_t sum = 0;
		max = min = *(param+0);//将第一个数赋值给max和min
		for(u8 i = 0;i < length;i++){
			if(max < *(param+i))//分别求出最大值和最小值
				max = *(param+i);
			else if(min > *(param+i))
				min = *(param+i);
			
			sum += *(param+i);//和
		}		
		return (sum - max - min)/(length - 2);		
	}else{
		s32 sum = 0;
		for(u8 i = 0;i < length;i++){
			sum += *(param+i);			
		}
		return sum/length;
	}
	
}

/*
 ******************************************************************************************************************************
 *	名    字：PutParam()
 *	参    数：struct ArryParam *variable, s16 param
 *	返回值  ：void
 *	功能描述：将param放到variable的数组中，以便后面利用
 *	建立时间：2016.10.06  茹凯琪
 *	备    注：每次修改时请标注修改理由，时间，姓名
 *			第一个元素表示最新放入的元素
 ******************************************************************************************************************************
*/

void PutParam(_ArryParamStruct *variable, s16 param)
{
	if(variable != NULL){
		for(u8 i = variable->length;i > 0 ;i--){
			variable->s16_param[i] = variable->s16_param[i-1];		
		}//第一个元素表示最新放入的元素
		variable->s16_param[0] = param;
		variable->length++;
		if(variable->length > variable->size){//超出范围时仅保存最新的size个数据
			variable->length = variable->size;
		}	
	}
}

/*
 ******************************************************************************************************************************
 *	名    字：ClearArry
 *	参    数：_ArryParamStruct *variable
 *	返回值  ：void
 *	功能描述：清空ArryParam的内容
 *	建立时间：2016.10.06  茹凯琪
 *	备    注：每次修改时请标注修改理由，时间，姓名
 *			
 ******************************************************************************************************************************
*/

void ClearArry(_ArryParamStruct *variable)
{
	if(variable != NULL && variable->length != 0){		
		memset(variable->s16_param,0,variable->length);
		variable->length = 0;
		return;
	}
}

/*
 ******************************************************************************************************************************
 *	名    字：GetAvgData()
 *	参    数：s16
 *	返回值  ：_ArryParamStruct *variable
 *	功能描述：对电流采用，处理相关参数的初始化
 *	建立时间：2017.03.11
 *	备    注：每次修改时请标注修改理由（要写明是什么，为什么，不要写怎么做），时间，姓名
 *			当某次传递进来的指针无效时返回上一次的值		
 *
 ******************************************************************************************************************************
*/
s16 GetAvgData(_ArryParamStruct *variable)
{
	static s16 lastData = 0;
	if(NULL != variable){
		s16 data;
		data = AvgFilter(variable->s16_param,variable->length);
		lastData = data;//保存当前值
		return data;	
	}else{
		return lastData;//当指针无效的时候返回上一次的值
	}
	
}















