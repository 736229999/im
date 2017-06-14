

/*=========myFn=========================================================*/



function formatData(data){
		var idFiled, textFiled, parentField;
		idFiled = 'id';
		textFiled = 'text';
		parentField = 'pid';
		var i, l, treeData = [], tmpMap = [];
		for (i = 0, l = data.length; i < l; i++) {
			tmpMap[data[i][idFiled]] = data[i];
		}
		for (i = 0, l = data.length; i < l; i++) {
			if (tmpMap[data[i][parentField]] && data[i][idFiled] != data[i][parentField]) {
				if (!tmpMap[data[i][parentField]]['children'])
					tmpMap[data[i][parentField]]['children'] = [];
				data[i]['text'] = data[i][textFiled];
				tmpMap[data[i][parentField]]['children'].push(data[i]);
			} else {
				data[i]['text'] = data[i][textFiled];
				treeData.push(data[i]);
			}
		}
		
		return treeData;
};

/*第一级渲染*/

function loadTreeforRoot(treeData){
	if(!treeData || treeData.length < 1) return;
		
		for(var i=0;i<treeData.length;i++){
			var data = [treeData[i]];
			
			var id = new Date().getTime();
			
			formatRoleTree(data,1,id);
			// $container.append('<div id="tree-wrap-'+id+'" class="roletree-wrap roletree"><ul id="tree-'+id+'"></ul></div>');
			
			
		}
}

	function formatRoleTree(nArr,currlevel,prefix){
		if(!nArr || nArr.length <= 0)return;
		if(!currlevel) currlevel = 1;
		
		for(var i=0;i<nArr.length;i++){
			var node = nArr[i];
			var sonArr = node.children;
			var formatSon = true;
			
			//如果有子节点则判断孙节点
			if(sonArr && sonArr.length > 0){
				for(var j=0;j<sonArr.length;j++){
					var son = sonArr[j];
					//如果子节点还有子节点则不进行格式化
					if(son.children && son.children.length > 0){
						formatSon = false;
						break;
					}
				}
			//没有子节点则不格式化子节点
			}else{
				formatSon = false;
			}
			
			if(formatSon){
				formatRoleNode(node,currlevel,true,prefix);
			}else{
				prefix = formatRoleNode(node,currlevel,false,prefix);
				formatRoleTree(node.children,currlevel + 1,prefix);
			}
		}


	};

/*分组子元素*/
function formatRoleNode(node,currlevel,formatChildren,prefix){
	node.text = renderRoleNode(node,currlevel == 1,prefix,currlevel);
	
	if(formatChildren){
		//分组子元素
		var sonArr = [],son = null,count = 0;
		
		for(var i=0;i<node.children.length;i++){
			var child = node.children[i];
			var cprefix = prefix + '-' + i;
			 
			 	sonArr.push({
			 		id : new Date().getTime(),
			 		text : ''
			 	});
			 	son = sonArr[sonArr.length - 1];
			
			
			son.text += renderRoleNode(child,false,cprefix,currlevel);
			
			count++;
		}
		
		node.children = sonArr;
	}
	return prefix;
}

/*渲染*/
function renderRoleNode(node,bold,prefix,currlevel){

	// var nodeHtml = '<span class="treeck">';
	// var nodeText = node.text;
	
	// if(roleOpt.type == 'ck'){
	// 	nodeHtml += '<input id="'+node.id+'" treeid="'+prefix+'" type="checkbox"';
		
	// 	if(node.attributes && node.attributes.checked){
	// 		nodeHtml += 'checked="true"';
	// 	}
	// 	nodeHtml += '"/>';
	// }
	
	// if(bold){
	// 	nodeText = '<b>'+nodeText+'</b>';
	// }
	
	// nodeHtml += '<label '+(roleOpt.type == 'ck' ? 'for="'+node.id+'"' : '')+'>'+nodeText+'</label>';
	// nodeHtml += '</span>';
	// return nodeHtml;

	// $('JS_TREE').append()

	var nodeHtml = '';
	var nodeText = node.text;

	if(currlevel==1){
		//渲染第一级
		nodeHtml+=''
				+'<li level='+currlevel+' id=m_'+node.id+' data_id='+node.id+' data_pid='+node.pid+'><a href="javascript:void(0);"><span class="tt">'+node.text+'</span>'
				+'<div class="operation_panel" style="display: none;">'
		        +'<ul>'
	            +'<li rel="creat">新建子部门</li>'
	            +'<li rel="rename">重命名</li>'
	            +'<li rel="delete">删除</li>'
	            +'</ul>'
		        +'</div>'
		        +'</li>';
		//渲染最后一集

        $('#JS_TREE .customLeftUl').append(nodeHtml);

	}else{
		appendChild(node,currlevel);
	}

	

}


function fenpiXuanranNoLi(currlevel,node){
	var nodeHtml = '';
	if(currlevel==3){
		//渲染第一级
		nodeHtml+=''
				+'<li level='+currlevel+' id=m_'+node.id+' data_id='+node.id+' data_pid='+node.pid+'><a href="javascript:void(0);"><span class="tt">'+node.text+'</span>'
				+'<div class="operation_panel" style="display: none;">'
		        +'<ul>'
	            +'<li rel="creat">新建子部门</li>'
	            +'<li rel="rename">重命名</li>'
	            +'<li rel="delete">删除</li>'
	            +'</ul>'
		        +'</div>'
		        +'</li>';
		//渲染最后一集

        
	}else{
		
		//渲染第一级
		nodeHtml+=''
				+'<li level='+currlevel+' id=m_'+node.id+' data_id='+node.id+' data_pid='+node.pid+'><a href="javascript:void(0);"><span class="tt">'+node.text+'</span>'
				+'<div class="operation_panel" style="display: none;">'
		        +'<ul>'
	            +'<li rel="creat">新建子部门</li>'
	            +'<li rel="rename">重命名</li>'
	            +'<li rel="delete">删除</li>'
	            +'</ul>'
		        +'</div>'
		        +'</li>';
		//渲染第二级
	}
		return nodeHtml;

}

function appendChild(node,currlevel){
	var str = '';
   $('#JS_TREE li[level="'+(parseInt(currlevel)-1)+'"][data_id="'+node.pid+'"]').each(function (){
   		var $target = $('ul[level]',this);
   		if($target.length){
   			str = fenpiXuanranNoLi(currlevel,node);
   			$target.append(str);
   		}else{
   			str = fenpiXuanran(currlevel,node);
   			$(this).append(str);
   			$(this).children('a').append('<b class="jstree-icon"></b>');
   		}
   });

}

function fenpiXuanran(currlevel,node){
	var nodeHtml = '';
	if(currlevel==3){
		//渲染第一级
		nodeHtml+='<ul level='+currlevel+'>'
				+'<li level='+currlevel+' id="m_'+node.id+'" data_id="'+node.id+'" data_pid='+node.pid+'><a href="javascript:void(0);"><span class="tt">'+node.text+'</span>'
				+'<div class="operation_panel" style="display: none;">'
		        +'<ul>'
	            +'<li rel="creat">新建子部门</li>'
	            +'<li rel="rename">重命名</li>'
	            +'<li rel="delete">删除</li>'
	            +'</ul>'
		        +'</div>'
		        +'</a></li>'
		        +'</ul>';
		//渲染最后一集
	}else{
		//渲染第一级
		nodeHtml+='<ul level='+currlevel+'>'
				+'<li level='+currlevel+' id="m_'+node.id+'" data_id="'+node.id+'" data_pid='+node.pid+'><a href="javascript:void(0);"><span class="tt">'+node.text+'</span>'
				+'<div class="operation_panel" style="display: none;">'
		        +'<ul>'
	            +'<li rel="creat">新建子部门</li>'
	            +'<li rel="rename">重命名</li>'
	            +'<li rel="delete">删除</li>'
	            +'</ul>'
		        +'</div>'
		        +'</a></li>'
		        +'</ul>';
		//渲染第二级
	}
	return nodeHtml;
}

