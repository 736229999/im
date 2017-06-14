seajs.use(['modules/feiniu', 'modules/util/generalServiceHelper', 'modules/util/generalUtil', 'JSON'],
    function (feiniu, serviceHelper, generalUtil, JSON) {
        var params = {
            objMerchantId: 71,    //商家主Id
            objSubMerchantId: 71,    //商家子账号Id
            objGoodsId: 490,        //商品Id
            objFlag: 1,           //枚举值  （商品详情、我的订单）
            storeinfoID: "J_store_info",   //店铺信息
            productInfoID: "J_product_info",  //商品详情
            orderInfoID: "J_order_info"      //我的订单
        };

        //页面环境变量
        var PageEnv = (function () {
            var config = function () {
                var currentHref = window.location.href;
                var currentDomain = currentHref.replace(new RegExp('^https?://([^/]+)/.+$'), '$1');
                var isBeta = false;
                if (currentDomain.indexOf('beta1.fn') > -1 || currentDomain.indexOf('beta.fn') > -1) {
                    isBeta = true;
                }

                if (isBeta) {
                    this.itemUrlBase = 'http://item.beta1.fn/';
                    this.storeUrlBase = 'http://storefront.beta1.fn/index-';
                } else {
                    this.itemUrlBase = 'http://item.feiniu.com/';
                    this.storeUrlBase = 'http://store.feiniu.com/index-';
                }
            };

            config.prototype = {
                constructor: config,
                getItemUrlBase: function () {
                    return this.itemUrlBase;
                },
                getStoreUrlBase: function () {
                    return this.storeUrlBase;
                }
            };

            return new config();
        })();

        var templateGoods =
            '<div id="J_product_info" class="pic" style="display:none;">\
                <a href="#" target="_blank"><img src="../../images/im/pic.jpg"  width="130" height="130"/></a>\
             <ul>\
                 <li class="on"><a class="lan" href="#" target="_blank">{title}</a></li>\
                 <li class="on1 ">成交笔数：</li><li class="on2 ">2100</li><li class="on3">笔</li>\
                 <li class="on ">评价（9889）</li>\
                 <li class="on2 price">{mallPrice}</li><li class="on3">元</li>\
             </ul>\
            <div class="buy"><a class="white" href="#" target="_blank">立即购买</a></div>\
            </div>';
        var templateOrder =
            '<div id="J_order_info" class="order" style="display: none;">\
               <ul>\
                   <li><a href="#" class="lan" target="_blank">订单号：2014070822012</a></li>\
                   <li>成交时间：2014-07-08</li>\
                   <li class="on2">实付款：</li><li class="on">56.02</li><li class="on1">元</li>\
               </ul>\
               <div class="pc">\
                   <a href="#" target="_blank"><img src="../../images/im/pic1.jpg"  border="0"/></a>\
                   <a href="#" target="_blank"><img src="../../images/im/pic2.jpg" /></a>\
                   <p>已发货  <a class="lan" href="#" target="_blank">查看物流</a></p>\
               </div>\
               <div class="btn_n"><a class="black" href="#" target="_blank">确认收货</a></div>\
           </div>';
        var templateRecommendProduct =
            '<div class="pcc"> ' +
            '<a href="#" target="_blank"><img height="77px" width="75px" src="static/images/pic3.jpg" border="0" /></a> ' +
            '<span><a class="black name" href="#" target="_blank">时尚韩国风衣</a></span> ' +
            '<span class="price">99.00</span><span>元</span> ' +
            '</div> ';
        var templateSliderControl =
            '<li id="J_slider_control$index"></li>';

        var EnvSniffer = (function () {
            var currentEnv = 'online';
            var currentHref = window.location.href;
            var currentDomain = currentHref.replace(new RegExp('^https?://([^/]+)/.+$'), '$1');

            if (currentDomain.indexOf('beta1.fn') > -1)/**beta环境**/{
                currentEnv = 'beta';
            } else if (currentDomain.indexOf('dev1.fn') > -1)/**dev环境**/{
                currentEnv = 'dev';
            }

            return {
                isOnline: function () {
                    return currentEnv === 'online';
                },
                isBeta: function () {
                    return currentEnv === 'beta';
                },
                getAddCartServiceHref: function () {
                    var self = this;
                    var serviceAddCartHref = '';

                    if (self.isOnline()) {
                        serviceAddCartHref = 'http://shop.feiniu.com/cartApi/addList';
                    } else if (self.isBeta()) {
                        serviceAddCartHref = 'http://buy.beta1.fn/cartApi/addList';
                    }

                    return serviceAddCartHref;
                },
                getCartIndex: function () {
                    var self = this;
                    var serviceAddCartHref = '';

                    if (self.isOnline()) {
                        serviceAddCartHref = 'http://shop.feiniu.com/cart/index';
                    } else if (self.isBeta()) {
                        serviceAddCartHref = 'http://buy.beta1.fn/cart/index';
                    }

                    return serviceAddCartHref;
                }
            }
        })();

        var im = {
            config: {
                SERVER: 'vender'
            },
            serviceCallbackMap: {},
            init: function () {
                this.registerServiceCallback();
                this.getparams();
                this.getData();
                this.bindEvent();
            },
            bindEvent: function () {
                var self = this;

                //响应聊天窗口左侧栏的切换事件
                window.refreshRight = function (param) {
                    var merchantID = param.merchantID;
                    var productID = param.productID;
                    var oldMerchantID = param.objMerchantId;
                    var oldProductID = param.objGoodsId;

                    if (!!merchantID) {
                        params.objMerchantId = merchantID;
                        params.objGoodsId = productID;
                        params.objFlag = parseInt(productID) > 0 ? 1 : 0;

                        if (oldMerchantID != merchantID) {
                            //不同商家，全部数据刷新
                            self.getData();
                        } else if (oldProductID != productID) {
                            //相同商家，不同商品,只刷新商品信息
                            self.getGoodsInfo();
                            self.initTab();
                        }
                    }
                };
            },
            registerServiceCallback: function () {
                $.extend(this.serviceCallbackMap, {
                    'store': function (result) {
                        if (result.success) {
                            var storeName = (result.data || {})['storeName'];
                            storeName = storeName == null ? '' : storeName;

                            if (!!storeName) {
                                window.parent.document.title = storeName;
                                window.parent.refreshTitleHandler && window.parent.refreshTitleHandler(storeName);

                                $(window.parent.document).find('#J_store_name').html(storeName);
                                $('#J_store_entrance').attr('href', PageEnv.getStoreUrlBase() + params.objMerchantId + '.html');
                            } else {
                                $(window.parent.document).find('#J_store_name').html("获取店铺信息失败~");
                            }
                        } else {
                            $(window.parent.document).find('#J_store_name').html("获取店铺信息失败~");
                        }
                    },
                    'merchant': function (result) {
                        if (result.success) {
                            var merchantInfo = (result.data || {})['merchantInfo'] || {};
                            $(document).find('#J_merchant_name').html(merchantInfo.merchantName);
                            !!merchantInfo.merchantId && (params.objMerchantId = merchantInfo.merchantId);
                        } else {
                            $(document).find('#J_merchant_name').html("获取商家信息失败~");
                        }
                    },
                    'good': function (result) {
                        var rightFrame = $(document);

                        if (result.success) {
                            var productInfo = (result.data || {})['goodsAsking'] || {};
                            productInfo.price = parseFloat(productInfo.price || productInfo.salePrice || 0).toFixed(2);
                            productInfo.productURL = PageEnv.getItemUrlBase() + productInfo.skuId;

                            rightFrame.find('#itemName').html(productInfo.title);
                            rightFrame.find('#itemPrice').html(productInfo.price);
                            rightFrame.find('#itemPic').attr('src', productInfo.pic01);
                            rightFrame.find('#comments').attr('href', productInfo.commentURL);
                            rightFrame.find('#J_product_info .item-buy').attr('href', productInfo.productURL);
                            rightFrame.find('#soldCount').html(productInfo.sold);
                            rightFrame.find('#commentCount').html(productInfo.commentCount);

                            var toAddProductString = JSON.stringify({
                                "cart_type": 0,
                                "is_simple": true,
                                "return_cart_data": false,
                                "product_list": [[{
                                    "sm_seq": params.objGoodsId,
                                    "kind": 1,
                                    "qty": 1,
                                    "price_snapshot": productInfo.price,
                                    "type": 1
                                }]]
                            });

                            //添加购物车
                            $('#J_product_info>.buy>.item-buy').html(productInfo.series ? '立即购买' : '加入购物车')
                                .unbind('click.buy').bind('click.buy', function (e) {
                                    //如果是系列品，跳转商详页
                                    if (productInfo.series) {
                                        generalUtil.openNewTab(productInfo.productURL);
                                    } else {//否则，加入购物车，并跳转购物车
                                        var cartWindow = window.open();

                                        serviceHelper.callJsonPService({
                                            url: EnvSniffer.getAddCartServiceHref(),
                                            data: {
                                                data: toAddProductString,
                                                source: window.location.href
                                            },
                                            success: function (resultInfo) {
                                                if (resultInfo.status == 1) {
                                                    cartWindow.location = EnvSniffer.getCartIndex();
                                                } else {
                                                    cartWindow.close();
                                                    alert('添加购物车失败：' + resultInfo.msg);
                                                }
                                            }
                                        });
                                    }

                                    e.preventDefault();
                                    return false;
                                });
                        } else {
                            rightFrame.find('#itemName') && rightFrame.find('#itemName').html("获取宝贝信息失败~");
                        }
                    },
                    'comment': function (result) {
                        if (result.success && result.data) {
                            var storeCommentInfo = result.data.storeCommentInfo;
                            if (!(storeCommentInfo instanceof Array && storeCommentInfo.length == 3)) {
                                return;
                            }

                            $('#J_store_info').children('.comment').each(function (index) {
                                var $this = $(this);

                                $this.children('.on2').children('b').html(storeCommentInfo[index].score.toFixed(1));
                                $this.children('.on3').html(Math.abs(storeCommentInfo[index].offset) + '%');
                                if (storeCommentInfo[index].offset < 0) {
                                    $this.children('img').attr('src', 'static/images/bottom.jpg');
                                }
                            });
                        }
                    },
                    'recommend': function (result) {
                        if (result.success && result.data) {
                            var productListInfo = result.data.recommendProductList;
                            if (productListInfo) {
                                var recommentContainer = $('#J_slider>.slider');
                                var slideController = $('#J_slider_control');
                                var lastContainer = null;

                                $('#J_slider').clearQueue().find('*').clearQueue();
                                slideController.clearQueue().find('*').clearQueue();
                                recommentContainer.children().remove();
                                slideController.children().remove();

                                for (var i = 1; i <= productListInfo.length; i++) {
                                    if (i % 2 == 1) {
                                        lastContainer = $('<li></li>');
                                        recommentContainer.append(lastContainer);
                                        slideController.append(templateSliderControl.replace(/\$index/, (parseInt(i / 2) + 1) + ""));
                                    }

                                    var productItem = productListInfo[i - 1];
                                    var product = $(templateRecommendProduct);
                                    product.find('a').attr('href', productItem['productURL']).attr('title', productItem['title']);
                                    product.find('img').attr('src', productItem['defaultPic']);
                                    product.find('.name').html(productItem['title']).attr('title', productItem['title']);
                                    product.find('.price').html(productItem['price']);

                                    lastContainer.append(product);
                                }

                                if (productListInfo.length % 2 == 1) {
                                    var prevContainer = lastContainer.prev();
                                    prevContainer.length == 0 && (prevContainer = lastContainer);
                                    lastContainer.prepend(prevContainer.find('div.pcc:last').clone());
                                }

                                if (productListInfo.length > 0) {
                                    $.event.trigger('INIT_SLIDER', {});
                                } else {
                                    $.event.trigger('HIDE_SLIDER', {});
                                }
                            }
                        }
                    }
                });
            },
            /**
             * 获取参数
             */
            getparams: function () {
                params.objMerchantId = getUrlParam("mid");
                params.objGoodsId = getUrlParam("pid");
                params.objFlag = getUrlParam("flag");
                params.storeinfoID = getUrlParam("sid");
                params.objSubMerchantId = getUrlParam("smid");
            },
            /**
             * 获取数据
             */
            getData: function () {
                if (!!params.objSubMerchantId) {
                    im.getInfoForPC(params.objSubMerchantId);
                } else {
                    im.getStoreInfo();
                    im.getMerchantInfo();
                    im.getCommentInfo();
                    im.getRecommendProductList();
                }

                this.initTab();
            },
            initTab: function () {
                $('#J_product').remove();
                $('#J_store').addClass('on');
                $('#J_store_info').show();
                $('#J_product_info').hide();

                //1是商品，2是订单
                if (params.objFlag == 1) {
                    $("#J_store").after("<p id=\"J_product\">商品信息</p>");
                    $('#J_product, #J_store').toggleClass('on');
                    $('#J_store_info').hide();
                    $('#J_product_info').show();
                    im.getGoodsInfo();
                } else if (params.objFlag == 2) {
                    //$("#J_store").after("<p id=\"J_order\">我的订单</p>");
                    //im.getOrderInfo(params.objGoodsId);
                }
            },
            getInfoForPC: function (subMerchantID) {
                var self = this;

                generalUtil.isNumber(subMerchantID) &&
                $.post(this.config.SERVER,
                    {
                        VENDER_INFO_TYPE: 'GET_STORE_RELATED_INFO_FOR_PC',
                        sub_merchant_id: subMerchantID
                    },
                    function (result) {
                        self.serviceCallbackMap['merchant'].call(self, result);
                        self.serviceCallbackMap['store'].call(self, result);
                        self.serviceCallbackMap['comment'].call(self, result);
                    },
                    'json'
                );
            },
            /**
             * 获取商品详情
             */
            getGoodsInfo: function () {
                var self = this;

                generalUtil.isNumber(params.objGoodsId) &&
                $.post(this.config.SERVER,
                    {
                        VENDER_INFO_TYPE: 'GET_PRODUCT_INFO',
                        product_id: params.objGoodsId
                    },
                    self.serviceCallbackMap['good'],
                    'json'
                );
            },
            /**
             * 获取商家信息
             */
            getMerchantInfo: function () {
                var self = this;

                generalUtil.isNumber(params.objMerchantId) &&
                $.post(self.config.SERVER,
                    {
                        VENDER_INFO_TYPE: 'GET_MERCHANT_INFO',
                        merchant_id: params.objMerchantId
                    },
                    self.serviceCallbackMap['merchant'],
                    'json'
                );
            },
            /**
             * 获取店铺信息
             */
            getStoreInfo: function () {
                var self = this;

                generalUtil.isNumber(params.objMerchantId) &&
                $.post(this.config.SERVER,
                    {
                        VENDER_INFO_TYPE: 'GET_STORE_NAME',
                        merchant_id: params.objMerchantId
                    },
                    self.serviceCallbackMap['store'],
                    'json'
                );
            },
            getRecommendProductList: function () {
                var self = this;
                $.post(this.config.SERVER,
                    {
                        VENDER_INFO_TYPE: 'GET_RECOMMEND_PRODUCT_LIST',
                        merchant_id: params.objMerchantId
                    },
                    self.serviceCallbackMap['recommend'],
                    'json'
                );
            },
            /**
             * 获取店铺评分信息
             */
            getCommentInfo: function () {
                var self = this;

                generalUtil.isNumber(params.objMerchantId) &&
                $.post(this.config.SERVER,
                    {
                        VENDER_INFO_TYPE: 'GET_STORE_COMMENT_INFO',
                        merchant_id: params.objMerchantId
                    },
                    self.serviceCallbackMap['comment'],
                    'json'
                );
            },
            /**
             * 获取订单详情
             */
            getOrderInfo: function (goodsId) {
                var productInfoID = params.productInfoID;
                $("#J_store_info").after(templateOrder);
                /*		 	 $("#J_store_info").after("<div  id=\"J_order_info\" class=\"order\" style=\"display: none;\">订单还没开发<div/>");
                 */

                /* 	 if(goodsId != null && goodsId != "") {
                 feiniu.fn.callService({
                 host: feiniu.config.itemHost,
                 method: 'feiniu.merchant.im.goods',
                 params: JSON.stringify({goodsId:goodsId}),
                 callback: function(data) {
                 if(null != data.result) {
                 var o = JSON.parse(data.result);

                 var order_info = feiniu.fn.renderTemplate(templateOrder, o);
                 alert(order_info);
                 $("#J_store_info").after(order_info);
                 }
                 }
                 });
                 }*/
            }
        }

        function getUrlParam(name) {
            var reg = new RegExp("(^|&)" + name + "=([^&]*)(&|$)"); //构造一个含有目标参数的正则表达式对象
            var r = window.location.search.substr(1).match(reg);  //匹配目标参数
            if (r != null) return unescape(r[2]);
            return null; //返回参数值
        }

        (function () {
            im.init.call(im);
        })();

    });