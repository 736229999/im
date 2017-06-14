// JavaScript Document

/*
* Discripe ： 页面左侧树机构
* Create   ：Damon.dai
*/

var toLeftBarTree={
	init:function(){
		
		this.tree();

		this.main();
		
	
	},

	main:function(){
		var $Ul=$('.customLeftUl');
		var $li=$('.customLeftUl>li');
		var $liSub=$('.customLeftUl>li>ul>li');
		var $a=$('.customLeftUl a');
		var $operation_panel=$('.customLeftUl li .operation_panel');
		var $jsTree=$(".customLeftUl");
		var $jstree_icon=$('.customLeftUl .jstree-icon');
		var $li_operation_panel=$('.operation_panel li');
		var onOff=true;
		
		//鼠标事件
		 /*$li.hover(function() {
		 	//$(this).children('a').addClass('leftLiSelected').siblings().removeClass('leftLiSelected');
			$(this).css('z-index','10')
         },function(){
			 $(this).css('z-index','0')
		})*/
		
		//鼠标事件
		 $a.on({mouseenter: function() {
		 		// $(this).addClass('leftLiSelected');
		 		$(this).children('.operation_panel').show();
             },mouseleave: function(e) {
		 		// $(this).removeClass('leftLiSelected');
		 		$(this).children('.operation_panel').hide();
             },click:function(){
			    //$(this).addClass('leftLiSelected').siblings().removeClass('leftLiSelected');
		     }
		 })

		 //alert($('.customLeftUl').find('a').length)
		 $('.customLeftUl').find('a').click(function(){
		 	$('.customLeftUl').find('a').removeClass('leftLiSelected');
		 	$(this).addClass('leftLiSelected');
		 })
		
		/*树操作面板*/
		$jstree_icon.on('click',function(){

				if(onOff){
					$(this).addClass('jstree-open').removeClass('jstree-close');
					$(this).parent().next('ul').show();//树面板 显示
				}else{
					$(this).addClass('jstree-close').removeClass('jstree-open');	
					$(this).parent().next('ul').hide();//树面板 显示
				}
				onOff=!onOff;
		})
		
		/*节点面板*/
		$operation_panel.on({mouseenter: function() {
				$(this).parent().closest('li').css('z-index',10);
				$(this).children().css('visibility','visible')
            },mouseleave: function() {
				$(this).parent().closest('li').css('z-index',0);
				$(this).children().css('visibility','hidden')
            }
		})
		
		$li_operation_panel.hover(function(){
			$(this).addClass('hover');
			//$(this).parent().closest('li').css('z-index',10);
			//$(this).parent().
		},function(){
			//$(this).parent().closest('li').css('z-index',0);
			$(this).removeClass('hover');
		});
		

		/*重命名*/
		$jsTree.on('click','[rel=rename]',function(){
				var $parent=$(this).parent().parent().parent();
				var $dom=$('<input class="reName" type="text" value="重命名" />');
				var $ttSpan=$parent.find('.tt');

				$parent.append($dom);
				//alert($(".reName").length)
				$(".reName").focus().blur(function(){			
					var inputVal = $(this).val();
					$ttSpan.html(inputVal);
					$dom.remove();
				}).keydown(function(e){
					if(e.keyCode==13){
						$(this).blur();
					}
				});
		});
		
		/*创建子节点*/
		$jsTree.on('click','[rel=creat]',function(){

            var sHtml=$('.leftLiSelected').clone().html();


            var lev=$('.leftLiSelected').parent().attr('level');


            console.log($(this).closest('li[id]'))//当前LI对象

            if(lev==1){
            	var htmlLev1=''
				+'<li level="2"><a><span class="tt">新建子部门</span><input class="reName" type="text" value="新建子部门" />'
	          	+'<div class="operation_panel" style="display: none;">'
	            +'<ul style="visibility: hidden;">'
	            +'<li rel="creat" class="">新建子部门</li>'
	            +'<li rel="rename">重命名</li>'
	            +'<li rel="delete">删除</li>'
	            +'</ul>'
	            +'</div>'
	            +'</a>'
	            +'</li>';

            	// $(htmlLev1).prependTo($('.leftLiSelected').parent().parent());
            	// $jsTree

            }else if(lev==2){
            	alert('lev2')

            	var htmlLev2=''
				+'<li level="3"><a><span class="tt">新建子部门</span><input class="reName" type="text" value="新建子部门" />'
	          	+'<div class="operation_panel" style="display: none;">'
	            +'<ul style="visibility: hidden;">'
	            +'<li rel="rename">重命名</li>'
	            +'<li rel="delete">删除</li>'
	            +'</ul>'
	            +'</div>'
	            +'</a>'
	            +'</li>';


            }            

            $(".reName").focus().blur(function(){
            	$('.leftLiSelected').find('.tt').html($('.leftLiSelected').find('.reName').val());
            	$('.leftLiSelected').find('.reName').remove();

            }).keydown(function(e){
					if(e.keyCode==13){
						$(this).blur();
					}
				});
		});
		
		
		
		/*删除*/
		$jsTree.on('click','[rel=delete]',function(){
				var $parent=$(this).parent().parent().parent();

				$parent.remove();
		});


		/*============================================================================*/
		
	

		
		
	},
	tree:function(){
		//alert()
		var data= [
			{id : '1',pid : '-1',text : '宝贝管理'},
			{id : '1-1',pid : '1',text : '宝贝操作权限'},
			{id : '1-1-1',pid : '1-1',text : '宝贝操作权限'},
			{id : '1-1-2',pid : '1-1',text : '宝贝操作权限'},
			{id : '1-1-3',pid : '1-1',text : '宝贝操作权限'},
			{id : '1-1-4',pid : '1-1',text : '宝贝操作权限'},
			{id : '1-1-5',pid : '1-1',text : '宝贝操作权限'},
			{id : '1-1-6',pid : '1-1',text : '宝贝操作权限'},
			{id : '1-2',pid : '1',text : '宝贝管理'},
			{id : '1-2-1',pid : '1-2',text : '宝贝管理'},
			{id : '1-2-1-1',pid : '1-2-1',text : '宝贝管理X'},
			{id : '1-2-1-2',pid : '1-2-1',text : '宝贝管理Y',attributes : {checked : true}},
			
			{id : '2',pid : '-1',text : '宝贝管理'}
		];

		var treeData=formatData(data);


		loadTreeforRoot(treeData);

	}
}