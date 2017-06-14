$(document).ready(function() {

  loadPopCont();
  function loadPopCont(){

    var oWrap = $(".popup_wrap .popup_cont");

    var cont1=$("#placeHold #p_1");
    var cont2=$("#placeHold #p_2");
    var cont3=$("#placeHold #p_3");
    var cont4=$("#placeHold #p_4");
    var cont5=$("#placeHold #p_5");
    var cont6=$("#placeHold #p_6");

    oWrap.html(cont1);
    //下一步
    $('body').on("click","#c_1",function(){
      oWrap.html(cont2);
    });

    $('body').on("click","#c_2",function(){
    oWrap.html(cont3);
    });
    $('body').on("click","#c_3",function(){
      oWrap.html(cont4);
      });
    $('body').on("click","#c_4",function(){
      oWrap.html(cont5);
      });
    $('body').on("click","#c_5",function(){
    oWrap.html(cont6);
    });

    //上一步
    $('body').on("click","#c_1_1",function(){
    oWrap.html(cont1);
    });
  $('body').on("click","#c_2_2",function(){
    oWrap.html(cont2);
    });
  $('body').on('click','#c_3_3',function(){
    oWrap.html(cont3);
    });
  $('body').on('click','#c_4_4',function(){
    oWrap.html(cont4);
    });
  $('body').on('click','#c_5_5',function(){
    oWrap.html(cont5);
    });
  }


});

$(document).ready(function() {
   
var dialog=myPopup({
"elem": ".popup_wrap",
"close":".popup_wrap .know"
})

  $("body").on("click",".popup_wrap .know",function(){
    dialog.closeFn();
  })
})
