#include "float.h"

/*
*stra: string address as mychar from char mychar[];
*float: float input like 12.345
*flen: fraction length as 3 for 12.345
*/


void py_f2s4printf(char * stra, float x, uint8_t flen)
{
	uint32_t base;
	int64_t dn;
	char mc[32];

	base = pow(10,flen);
	dn = x*base;
	sprintf(stra, "%d.", (int)(dn/base));
	dn = abs(dn);
	if(dn%base==0)
	{
		for(uint8_t j=1;j<=flen;j++)
		{
			stra = strcat(stra, "0");
		}
		return;
	}
	else
	{
		if(flen==1){
			sprintf(mc, "%d", (int)(dn%base));
			stra = strcat(stra, mc);
			return;
		}

		for(uint8_t j=1;j<flen;j++)
		{
			if((dn%base)<pow(10,j))
			{
				for(uint8_t k=1;k<=(flen-j);k++)
				{
					stra = strcat(stra, "0");
				}
				sprintf(mc, "%d", (int)(dn%base));
				stra = strcat(stra, mc);
				return;
			}
		}
		sprintf(mc, "%d", (int)(dn%base));
		stra = strcat(stra, mc);
		return;
	}
}

void my_sprintf(char *result, const char *prefix, float num, uint8_t flen)
{
    char temp[32];  // 临时存储浮点数转换结果

    // 第一步：转换浮点数为字符串
    py_f2s4printf(temp, num, flen);

//    // 第二步：将前缀和浮点数结果组合
//    // 方法1：使用简单的字符串拼接
//    result[0] = '\0';  // 清空结果字符串
//    strcat(result, prefix);  // 添加前缀
//    strcat(result, temp);    // 添加浮点数字符串

    // 方法2（更简单）：直接用sprintf组合（如果你只是想显示，不担心代码大小）
     sprintf(result, "%s%s", prefix, temp);
}

