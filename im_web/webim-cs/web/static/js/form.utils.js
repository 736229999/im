//超链接乱码问题
//调用:<a href="" onclick="return linkClick(this)"></a>
 function linkClick(linkObject) {               
    var formObject = document.createElement('form');   
    document.body.appendChild(formObject);   
     formObject.setAttribute('method', 'post');   
     var url = linkObject.href;   
     var uri = '';   
     var i = url.indexOf('?');   
               
     if(i == -1) {   
        formObject.action = url;   
     } else {   
        formObject.action = url.substring(0, i);
     }   
               
     if( i >= 0 && url.length >= i + 1) {   
        uri = url.substring(i + 1, url.length);   
     }   
  
     var sa = uri.split('&');   
               
     for(var i = 0; i < sa.length; i++) {   
       var isa = sa[i].split('=');         
       var inputObject = document.createElement('input');   
       inputObject.setAttribute('type', 'hidden');   
       inputObject.setAttribute('name', isa[0]);   
       inputObject.setAttribute('value', isa[1]);   
       formObject.appendChild(inputObject);   
     }                 
     formObject.submit();             
     return false;  
 }
 //表格选中换颜色
 function changeColor(){
	 var table = document.getElementById("_pageList");
	 var tabletr = table.getElementsByTagName("tr");
	for(var i=0;i<tabletr.length;i++){
		if(i%2==0){
		   tabletr[i].className="trgb2";
	       tabletr[i].name='cc';
		}
		tabletr[i].onmouseover=function(){
			if(this.name == "aa"){
				this.className="trgb";	
			}else{
				this.className="trgb1";	
			}
		};
		tabletr[i].onmouseout=function(){
			if(this.name == "aa"){
				this.className="trgb";	
			}else if(this.name=="cc"){
				this.className="trgb2";
			}else{
			  	this.className="";
			}
		};
		tabletr[i].onclick=function(){
			var trsilb=this.parentNode.getElementsByTagName("tr");
			for(var a=0;a<trsilb.length;a++){
	            if(a%2==0){
	                trsilb[a].name="cc";
	                trsilb[a].className="trgb2";
	            }else{
	                 trsilb[a].name="";
	                trsilb[a].className="";
	            }	
			}
			this.name="aa";
			this.className="trgb";	
		};
	}
}
//遍历form,把form值放入一个数组里并返回(参数：form的id值)
function getFormToJson(frmID) { 
	 var a = [];
     var obj = document.getElementById(frmID); 
     if (obj == null) return ""; 
     var i; 
     var item; // for each form's object 
     var itemValue; // store each form object's value 
     for (i = 0; i < obj.length; i++) { 
             item = obj.elements[i]; // get form's each object 
             if (item != null && item.name != undefined && item.name != '') { 
                 if (item.type == 'select-one') { 
                     itemValue = item.options[item.selectedIndex].value; 
                 } 
                 else if (item.type == 'checkbox' || item.type == 'radio') { 
                     if (item.checked == false) { 
                         continue; 
                     } 
                     itemValue = item.value; 
                 } 
                 else if (item.type == 'button' || item.type == 'submit' || item.type == 'reset' || item.type == 'image') { 
                     continue; 
                 } 
                 else { 
                     itemValue = item.value; 
                 } 
                 if(itemValue && itemValue!==''){
                	 a.push('"' + item.name + '":"' + itemValue + '"');
                 }
                 //itemValue = encodeURIComponent(itemValue); 
                 
          } 
    } 
	return a + "";
} 

function getParamStringByIds(ids) { 
	if(ids==null||ids.length==0)return "";
	var ps = ids[0]+'='+document.getElementById(ids[0]).value; 
	for(var i=1;i<ids.length;i++){
		ps += '&' +ids[i]+'='+document.getElementById(ids[i]).value; 
	}
	return ps;
}

//遍历form,清空form里的值
function clearForm(formID) { 
    var obj = document.getElementById(frmID); 
    obj.reset();
}
//获取form的json值
function getFormJson(formObj) {
	var fields = $(formObj).serializeArray();
	var a = [];
	for ( var i = 0; i < fields.length; i++) {
		var f = fields[i];
		if ($.trim(f.value) != "") {
			a.push('"' + f.name + '":"' + $.trim(replaceJson(f.value)) + '"');
		}
	}
	return a + "";
}



//判断checkbox是否没有 任何选中以及是否选中多个(废弃)
function judgeCheckbox(checkboxName){
	var $allSelected=$("input[name="+checkboxName+"]:checked");
	var length=$allSelected.length;
	if(length==0){
		alert("必须选中一个进行删除");
		return false;
	}else if(length>=2){
		alert("无法同时删除多个");
		return false;
	}
	return $allSelected;

}

//废弃
function getSelectedCheckboxValue($allSelected){
	var id;
	$allSelected.each(function(){
		id=$(this).val();
	});
	return id;
}

//只能同时选中一个checkbox并且返回对应的id值
function selectOnlyCheckbox(obj){
	var checkboxName=obj.name;
	var origanVal = obj.checked;
	var allCheckbox=$("input[name="+checkboxName+"]:checked");
	for(var i=0;i<allCheckbox.length;i++){
		allCheckbox[i].checked=false;
	}
	obj.checked=origanVal;
}

//通过checkbox名称得到对应的ID值
function getCheckBoxID(checkboxName){
	var $allSelected=$("input[name="+checkboxName+"]:checked");
	var id;
	$allSelected.each(function(){
		id=$(this).val();
	});
	return id;
	
}

function getCheckBoxIDs(checkboxName){
	var $allSelected=$("input[name="+checkboxName+"]:checked");
	var ids = [];
	$allSelected.each(function(){
		ids.push($(this).val());
	});
	return ids;
}

function selectAllCheckBox(selectAllCheckbox,checkboxName){
	var allCheckbox=$("input[name="+checkboxName+"]");
	for(var i=0;i<allCheckbox.length;i++){
		allCheckbox[i].checked=selectAllCheckbox.checked;
		allCheckbox[i].onchange = function(){
			selectAllCheckbox.checked=ifSelectAll(checkboxName);
		};
	}
}

function setSelectAllCheckBox(selectAllCheckbox,checkboxName){
	$("input[name="+selectAllCheckbox+"]").checked=ifSelectAll(checkboxName);
}

function ifSelectAll(checkboxName){
	var allCheckbox=$("input[name="+checkboxName+"]");
	for(var i=0;i<allCheckbox.length;i++){
		if(!allCheckbox[i].checked){
			return false;
		}
	}
	return true;
}


function ifSelected(checkboxName){
	var $allSelected=$("input[name="+checkboxName+"]:checked");
	var len=$allSelected.length;
	var flag;
	if(len==0){
		flag=false;
	}else{
		flag=true;
	}
	return flag;
}

