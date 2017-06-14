
/**
 * 
 */
//var ajax = false;

/**
 * ajax异步js类
 */
var ajaxUtils = {
	//ajax : {},
	
	/**
	 * 初始化异步对象（AJAX）
	 */
	init : function() {
		if(window.XMLHttpRequest) {
			return ajaxUtils.ajax = new XMLHttpRequest();
		} else if(window.ActiveXObject) {
			try {
				return ajaxUtils.ajax = new ActiveXObject("Msxml2.XMLHTTP");
			} catch (e) {
				try {
					return ajaxUtils.ajax = new ActiveXObject("Microsoft.XMLHTTP");
				} catch (e) {
					//提示浏览器不支持ajax
					return false;
				}
			} 
		}
	},
	
	/**
	 * 简单的POST方法 
	 * @param url 服务器地址
	 * @param params 参数
	 * @param callback 回调函数
	 * @param exdt 异常返回格式 json jsp控制SpringMVC异常返回格式
	 */
	post : function(url, params, callback, isAjax,exdt) {
		var ajax = ajaxUtils.init();
		var ajaxboolean = isAjax;
		if(!ajax){  
	        return alert('create ajax failed!');  
	    }
		ajax.open("POST", url, ajaxboolean);
	    //如果传入的回调函数为空，则执行默认的回调函数
	    if(callback) {
	    	ajax.onreadystatechange = function() {
	    		if(ajax.readyState == 4) { 
	    	        if(ajax.status == 200) {
	    	        	callback(ajax.responseText);
	    	        }else if(ajax.status == 401) {//登录跳转
	    	        	var data=eval('('+ajax.responseText+')');
	    	        	window.location.href = data.url;
	    	        }else {
	    	        	return false;
	    	        }
	        	}  
	    	};
	    } else {
	    	ajax.onreadystatechange = ajaxUtils.callback;
	    }
	    ajax.setRequestHeader("Content-Type","application/x-www-form-urlencoded");  
	    if(!exdt||exdt!="jsp"){
	    	exdt = "json";
	    }
	    ajax.setRequestHeader("Exception-DataType",exdt);
	    
	    ajax.send(params); 
	},
	
	/**
	 * 默认的回调函数
	 */
	callback : function() {
		if(ajax.readyState == 4) {  
	        if(ajax.status == 200) {  
	            return ajax.responseText;
	        } else {
	        	return false;
	        }
    	}  
	}
};

/**
 * 页面加载时初始化(这个方法不可行，因为每次异步对象不一样)
 */
//window.onload = ajaxUtils.init();
/*
var okFunc = function(){  
    if(xmlHttp.readyState == 4) {  
        if(xmlHttp.status == 200) {  
        	alert(xmlHttp.responseText);
            //$("#msg").html(xmlHttp.responseText);  
        }  
    }  
};
var startAjax = function(){  
    $("#msg").html("Loading...");  
    createXMLHttpRequest();  
     
};
*/