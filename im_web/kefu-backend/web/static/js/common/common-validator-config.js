/**
 * <pre> 
 * @fileoverview
 *                      Copyright (c) www.MarsClan.org
 * ============================================================================
 * JavaScript   : 通用验证器配置文件
 * Version      : 2.1
 * Author       : 啸白腥
 * Date         : 2012-06-17
 * Dependences  : common-validator-2.x.js
 * Description  : 
 * 
 * ------------------------- Revision of history ------------------------------
 *  啸白腥@2012-06-17: 修改内容
 * 
 * 
 * ============================================================================
 * </pre>
 **/
(function($) {
  /**
   * 配置信息常量
   */
  var Global = {
    
    /**
     * 错误信息提示方式.
     */
    showErrorType : {
      ALERT : 1, //原始alert提示
      PANLE : 2, //在信息面板中集中显示错误信息
      STYLE : 3, //在字段的title提示上显示错误信息，并设置字段的样式
      LABEL : 4, //在字段尾部显示错误信息
      DIV   : 5  //在字段下方显示错误信息
    }, 
    
    /**
     * 验证登记模式.
     */
    registerType : {
      ADD       : 0, //添加模式
      FORM      : 1, //表单生成模式
      ADD_FORM  : 2  //全模式。支持添加和表单两种模式
    }
  };
  
  $.Config = {
    isFocus                : true, //验证失败后是否会落焦点 
    titleSepartor          : ",", 
    enableSubValidator     : true, 
    
    registerType           : Global.registerType.ADD_FORM, 
    validateProperty       : "class", //通过控件元素进行添加验证时的元素属性
    
    isManyErrorMessage     : false, //验证失败后，错误信息是否一块显示
    showErrorType          : Global.showErrorType.ALERT,
    errorMesssageClass     : "error"
  };
})(jValidator);