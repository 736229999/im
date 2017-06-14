/**
 @describe 弹窗 popup-v3
 @create By Damon.Dwq
 @contact qq361904805
 @time 2015-07-27 17:04:32
 @last time 2015-8-5 10:54:17
 @========使用API==========
    调用：var p1 = new Popup({
            title : '公告1',
            content : $('#cont')
        });

    参数方法：

    options :
        title 标题文本
        content 内容 可以是jq对象 字符串
        dir:center||right
        buttons:json:{okVal,okFn} 确认，取消按钮
        beDrag : true||false 可拖拽
        mask : true||false
        sign : 弹窗唯一标识，多个弹窗时需加上

    method : createCont closeFn maskFn

 **/


   Popup = function(opts){
        this.self = $(this);
        this.settings = {
            mask : true,
            title : '公告',
            content : '我是弹窗内容',
            dir : 'center',
            /*buttons : {
                okVal : '确认',
                okFn : function(){alert('okFn')},
                cancelVal : '取消',
                cancelFn : function(){alert('cancelFn')}
            },*/
            beDrag : true,
            sign : ''
        };

        this.oMask = $('<div id="mask"></div>');

        this.izIndex = 999;


        $.extend( this,this.settings , opts||{} );


        if( this.flagJson[this.sign] == undefined ){
            this.flagJson[this.sign] = true;
        }

        if( this.flagJson[this.sign] ){

            this.handle();
            this.flagJson[this.sign] = false;
        }

   };

   Popup.prototype.flagJson = {};

   Popup.prototype.handle = function(){

        var This = this;

        $(this).on('beforePop',this.beforePopFn);



        this.maskFn();
        this.createCont();
        this.setPos();


        this.beDrag && this.dragFn();

        $(window).on('resize', function(){
            This.setPos();
        });

        $('[js-hook="shutdown"]').on('click',function(){
            This.closeFn();
        });



   };

   Popup.prototype.createCont = function(){


        if( this.content instanceof jQuery ){
           this.content = this.content.prop('outerHTML');
        }
		
        var popTpl = '<div data-id="'+this.sign+'" class="ui_pop">'
                        +'<table class="ui_pop_grid">'
                        +    '<tr>'
                        +        '<td js-hook="popup_drag" class="ui_pop_head">'
                        +            '<a href="javascript:;" js-hook="shutdown" class="ui_pop_close">&#215;</a>'
                        +            '<div class="ui_pop_title">'
                        +                this.title
                        +             '</div>'
                        +        '</td>'
                        +    '</tr>'
                        +    '<tr>'
                        +        '<td class="ui_pop_body">'
                        +            '<div class="ui_pop_cont">'
                        +                this.content
                        +            '</div>'
                        +        '</td>'
                        +    '</tr>'
                        +    '<tr>'
                        +        '<td class="ui_pop_footer">'
                        +            '<div class="ui_pop_btns">'
                        +            '</div>'
                        +        '</td>'
                        +    '</tr>'
                        +'</table>'
                        +'</div>';
        this.oWrap = $(popTpl);

        this.oWrap.appendTo($('body'))

        this.createBtns();//创建按钮
   };

   Popup.prototype.maskFn = function(){

        if(this.mask != true) return false;

        if (!this.mask) return false;

        if(!$("#mask")[0]){
            $("body").append(this.oMask);
        }else{
            this.oMask = $("#mask");
        }

        this.oMask.css({
            position : 'absolute',
            top : '0',
            width : '100%',
            height : '100%',
            backgroundColor : '#000',
            opacity : 0.5
        })
   };

   Popup.prototype.setPos = function(obj){
        var $obj = this.oWrap;

        switch(this.dir){
            case  'center':
            var _l = ( $(window).width() - $obj.width() ) / 2;
            var _t = ( $(window).height() - $obj.height() ) / 2;
            break;

            case  'right':
            var _l = $(window).width() - $obj.outerWidth();
            var _t = $(window).height() - $obj.outerHeight();
            break;

            case  'follow':
            console.log("暂时没做")
            break;

        }
        if(!_t && !_l){return false}
        console.log(_t,$(window).width(),$obj.width())
        $obj.css({
            position : 'absolute',
            left : _l,
            top : _t,
            zIndex : this.izIndex
        });
   };

   Popup.prototype.closeFn = function(){

        this.oWrap.remove();
        if($('.ui_pop').length == 0 ) this.oMask.remove();

        this.flagJson[this.sign] = true;
   };



   Popup.prototype.createBtns = function(){
        var This = this,sOkval = null,sCancelVal = null,json = this.buttons;

        if( !this.buttons ) return false;//没有buttons 参数就不执行以下

        ergodic(json);//解析buttons 传进来的参数

        var oBtnWrap = this.oWrap.find('.ui_pop_btns');
        var tplOk ='<a class="ui_pop_btn_orange ui_pop_btn" href="javascript:void(0);" js-hook="ok">'+sOkval+'</a>';
        var tplCancel = '<a js-hook="shutdown" class="ui_pop_btn_white ui_pop_btn ui_pop_space" href="javascript:void(0);" js-hook="cancel">'+sCancelVal+'</a>';

        oBtnWrap.html(tplCancel+tplOk);

        this.oWrap.find('[js-hook=ok]').on('click',function(){
            okFn();
        });
        this.oWrap.find('[js-hook=cancel]').on('click',function(){
            This.closeFn();
        });


        function ergodic(json){
            for(var attr in json){
                switch(attr){
                    case  'okVal':
                        sOkval = json.okVal;
                    break;

                    case  'okFn':
                        okFn = json.okFn;
                    break;

                    case  'cancelVal':
                        sCancelVal = json.cancelVal;
                    break;

                    case  'cancelFn':
                        cancelFn = json.cancelFn;
                    break;
                };
            }
        };
   };

   Popup.prototype.dragFn = function(){

    var oTrigger = this.oWrap.find('.ui_pop_title');
    var oPanel = this.oWrap;
    var zIndex = this.izIndex;

    oTrigger.css({cursor : 'move'});

    oTrigger.on('mousedown',function(ev){

        oPanel.css('zIndex',zIndex++);

        var disX = ev.pageX - oPanel.offset().left;
        var disY = ev.pageY - oPanel.offset().top;

        $(document).on({
            'mousemove.foo':function(ev){
                        oPanel.css({
                            left : ev.pageX - disX,
                            top  : ev.pageY - disY
                        })
            },
            'mouseup.foo' : function(ev){
                 $(document).off('.foo')
            }

        });

        return false;

    });

    //双击加高面板层级
    oTrigger.dblclick(function(){
        oPanel.css('zIndex',zIndex += 10);
    });

   };



