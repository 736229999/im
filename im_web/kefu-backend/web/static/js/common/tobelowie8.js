/*
 *  name  ie6,ie7兼容提示    
 *  Copyright feiniu.ued       
 *  Designed and built by Damon.daiwenqi  
 *  date 2015-01-23 16:43:08          
 *  update 2015-01-23 16:43:08        
 */
(function(){

var RE_NON_WORD = /\W/g
var doc = document
var head = document.getElementsByTagName('head')[0] ||
    document.documentElement
var styleNode

var importStyle = function(cssText, id) {
  if (id) {
    // Convert id to valid string
    id = id.replace(RE_NON_WORD, '-')

    // Don't add multiple times
    if (doc.getElementById(id)) return
  }

  var element

  // Don't share styleNode when id is spectied
  if (!styleNode || id) {
    element = doc.createElement('style')
    id && (element.id = id)

    // Adds to DOM first to avoid the css hack invalid
    head.appendChild(element)
  } else {
    element = styleNode
  }

  // IE
  if (element.styleSheet !== undefined) {

    // http://support.microsoft.com/kb/262161
    if (doc.getElementsByTagName('style').length > 31) {
      throw new Error('Exceed the maximal count of style tags in IE')
    }

    element.styleSheet.cssText += cssText
  }
  // W3C
  else {
    element.appendChild(doc.createTextNode(cssText))
  }

  if (!id) {
    styleNode = element
  }
}




var cssStyle=
		'.ie6fixedTL{position:absolute;_top:expression(eval(document.documentElement.scrollTop-60))}'
		+'.toBelowie8'
		+'{'
		+'width: 100%;'
		+'padding: 14px 15px;'
		+'margin: 0 auto;'
		+'background: #8c8e8c;'
		+'color: #fff;'
		+'font-size: 14px;'
		+'position: fixed;'
		+'top: 0;'
		+'_position:absolute;'
		+'text-align: center;'
		+'z-index:9;'
		+'}'
		+''
		+'.toBelowie8 a{color:#fff;text-decoration:none;background:#0086e7;}'
		+'.toBelowie8 a:hover{color:#fff;text-decoration:underline;background:#84aaff;}'
		+'.toBelowie8 .ie{position: relative;top:0;height:20px;line-height:20px;width: 113px;text-align:center;display: inline-block;*display:inline;*zoom:1;border:1px solid #c7d7fa;}'
		+'.toBelowie8 .ie .icon{position:absolute;left:0;top:-6px;background: url(img/icon-ie.jpg) no-repeat;height: 33px;width: 113px;cursor: pointer;}'
		+'.toBelowie8 .ie:hover .icon{background: url(img/icon-ie-hover.jpg) no-repeat;}'
		+'.toBelowie8 .chrome{position: relative;top:0;height:20px;line-height:20px;width: 113px;text-align:center;display: inline-block;*display:inline;*zoom:1;border:1px solid #c7d7fa;}'
		+'.toBelowie8 .chrome .icon{position:absolute;left: 2px;top:2px;background: url(img/icon-chr.gif) no-repeat;width: 16px;height: 16px;}'

		+'.toBelowie8 .ui-dialog-close {'
		+'color: #FFF;'
		+'cursor: pointer;'
		+'display: block;'
		+'font-family: tahoma;'
		+'font-size: 32px;'
		+'font-weight: 400;'
		+'display: block;'
		+'height: 20px;'
		+'width: 20px;'
		+'line-height: 20px;'
		+'text-align: center;'
		+'position: absolute;'
		+'right: 25px;'
		+'text-decoration: none;'
		+'top: 15px;'
		+'z-index: 10;'
		+'}'


		var html=''
		+'<div class="toBelowie8 ie6fixedTL">'
		+'很抱歉，系统不支持IE6/IE7，我们推荐您： 点击下载 '
		+'<a class="ie" href="http://www.microsoft.com/china/windows/IE/upgrade/index.aspx?spm=686.1000923.0.0.ryYdZK&file=index.aspx" target="_blank">'
		+'<span class="icon"></span>升级IE浏览器'
		+'</a>'
		+'&nbsp;或点击下载&nbsp;'
		+'<a class="chrome" href="http://www.google.cn/intl/zh-CN/chrome/browser/?spm=1.7274553.0.0.UKLD1E" target="_blank">'
		+'<span class="icon"></span>chrome浏览器'
		+'</a>'

		+'</div>'

		var styleTag = document.createElement('style');
		var divWrap = document.createElement('div');
		var closeNode =document.getElementById('js_ie67_close');


		importStyle(cssStyle);

	    divWrap.innerHTML=html;

	    document.getElementsByTagName('body')[0].appendChild(divWrap);

}());