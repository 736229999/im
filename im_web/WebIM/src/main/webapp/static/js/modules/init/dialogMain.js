/* ============================== */
/*
author      : HQY
date        : 2014-11-07
qq          : 363395863
email       : hqy332211@163.com
 */
/* ============================== */

seajs.use([
	'$',
	'modules/dialogControl'
], function (
	jq,
	dialogControl
) {
$(function(){
	console.log(dialogControl);
	dialogControl.refresh({
		onSendingMessage : function(html){
			dialogControl.appendMessage({
				type : 'buyer_msg',
				html : html,
				time : '2014-11-07 12:23:22',
				name : 'sss'
			});
		}
	});
	
	
});
});