#include"unmarshal_remote_res.h"
#include"remoteop.h"
#include<string.h>
#include<arpa/inet.h>
void unmarshal_open_res_msg(void *ser_res, int sz,rem_res_t *res){
	int processed = 0 ; 
	int rem_op,ret_val,err_no;
	
	/* Copy operation copde for which this response is  */
	memcpy(&rem_op,ser_res+processed,sizeof(int));
	processed +=sizeof(int);
	res->rem_op = ntohl(rem_op);
	/* Copy return value of the open call */
	memcpy(&ret_val,ser_res+processed,sizeof(int));
	processed +=sizeof(int);
	res->u.open_res.ret_val = ntohl(ret_val);
	/* Copy error code if there was error  */
	memcpy(&err_no,ser_res+processed,sizeof(int));
	processed +=sizeof(int);
	res->u.open_res.err_no = ntohl(err_no);
	
}
