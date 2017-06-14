(function($) {
  $.messages = {
      
    // ~ Validator Messages
    // ---------------------------------------------------------------------------------
    checkNotEmpty         : "[{title}] 不可为空!",
    checkNotBlank         : "[{title}] 不可为空白!", 
    checkNumber           : "[{title}] 必须为数值型!",
    checkInteger          : "[{title}] 必须为整数!", 
    checkNInteger         : "[{title}] 必须为正整数!", 
    checkFloat            : "[{title}] 必须为浮点数!", 
    checkNFloat           : "[{title}] 必须为正浮点数!",
    checkCustomNumber     : "[{title}] 只能输入{0}位整数{1}位小数!", 
    checkDateCompare      : "[{title}] 起始值不可大于结束值!",
    checkMaxByteLength    : "[{title}] 最多为{0}个字节!",  
    checkMaxLength        : "[{title}] 最多为{0}个字符!", 
    checkMinLength        : "[{title}] 最少为{0}个字符!", 
    checkBetweenLength    : "[{title}] 长度必须在{0}～{1}个字符之间!", 
    checkBetweenByteLength: "[{title}] 长度必须在{0}～{1}个字节之间!",
    checkRegular          : "[{title}] 格式错误，{0}!",
    checkEnglish          : "[{title}] 只可为英文字母!",  
    checkEngAndNum        : "[{title}] 只可为英数字!", 
    checkWordChar         : "[{title}] 只可为英数字和底线!",
    checkWordChar         : "[{title}] 只可为“-”、“_”、空格、字母和数字!",
    checkMustSelected     : "请选择 [{title}]!", 
    checkCustomFunction   : "输入 [{title}]错误!", 
    checkEmail            : "[{title}]输入格式错误，请输入合法Email地址!", 
    checkURL              : "[{title}]输入格式错误，请输入合法URL地址!",
    dateStyle             : "[{title}]输入格式错误",
    
    // ~ Code Messages
    // ---------------------------------------------------------------------------------
    C001 : "[{title}]自定义验证信息码!"
        
  };
})(jValidator);
