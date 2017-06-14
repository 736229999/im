$(function(){
    var sidebar;

	function resize(){
		$('#sidebar').height($(window).height());
		if(!sidebar){
			sidebar = $('#sidebar').fnscrollbar({
				sizethumb : 200
			});
		}else{
			sidebar.fnscrollbar('update');
		}
	};
	
	$('#sidebar .fnj-scrollbar-scrollbar').hover(function(){
		$(this).addClass('fnj-scrollbar-scrollbar-hover');
	},function(){
		$(this).removeClass('fnj-scrollbar-scrollbar-hover');
	});
	
	resize();
	$(window).bind('resize.base',resize);
});