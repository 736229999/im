/**
 * Author: morningking
 * Date: 2016/1/4
 * Contact: 243717042@qq.com
 */
seajs.use([
    'modules/feiniu',
    'modules/util/generalUtil'
], function (feiniu, generalUtil) {
    var IMSeller = (function () {
        //页面环境变量
        var PageEnv = (function () {
            var config = function () {
                var url = window.location.href;
                this.productID = generalUtil.getQueryValue(url, "productID") || 0;
                this.merchantID = generalUtil.getQueryValue(url, "merchantID") || 0;
                this.customerToken = generalUtil.getQueryValue(url, "customerToken");
                this.serviceId = generalUtil.getQueryValue(url, "serviceId") || 0;
                this.userComment = decodeURI(generalUtil.getQueryValue(url, "userComment") || '');
                this.isOppositeCustomer = generalUtil.getQueryValue(url, "isOppositeCustomer") == 'true' || false;

                var currentHref = window.location.href;
                var currentDomain = currentHref.replace(new RegExp('^https?://([^/]+)/.+$'), '$1');
                var isBeta = false;
                if (currentDomain.indexOf('beta1.fn') > -1 || currentDomain.indexOf('beta.fn') > -1) {
                    isBeta = true;
                }

                if (isBeta) {
                    this.itemUrlBase = 'http://item.beta1.fn/';
                    this.merchantUrlBase = 'http://order.beta.fn.com/'
                } else {
                    this.itemUrlBase = 'http://item.feiniu.com/';
                    this.merchantUrlBase = 'http://merchant.feiniu.com/';
                }
            };

            config.prototype = {
                constructor: config,
                checkIsOppositeCustomer: function () {
                    return this.isOppositeCustomer;
                },
                getCrpUrlBase: function () {
                    return 'crp';
                },
                getUrlBase: function () {
                    return "vender";
                },
                getToken: function () {
                    return this.customerToken;
                },
                getServiceId: function () {
                    return this.serviceId;
                },
                getProductID: function () {
                    return this.productID;
                },
                getMerchantID: function () {
                    return this.merchantID;
                },
                getItemUrlBase: function () {
                    return this.itemUrlBase;
                },
                getDeliveryUrlBase: function () {
                    return this.merchantUrlBase;
                },
                getUserComment: function () {
                    return this.userComment;
                }
            };

            return new config();
        })();

        //全局事件类型
        var GlobalEventType = {
            /**更新 订单摘要信息**/
            RENDER_ORDER_SUMMARY_INFO: 'IMSELLER#RENDER_ORDER_SUMMARY_INFO'
        };
        //全局事件分发器
        var GlobalEventManager = {
            registerEvent: function (event, fn, context) {
                $(document).bind(event, function (event, param) {
                    fn.call(context, param);
                });
            },
            dispatchEvent: function (event, params) {
                $.event.trigger(event, params);
            }
        };

        //客户信息标签页管理器
        var CustomerManager = (function () {
            var hasRenderedBefore = false;
            var isOnRetrieveProgress = false;
            var customerTab = $('.customer-tab');
            var orderRecordPanel = customerTab.find('.Record');

            return {
                init: function () {
                    this.initUI();
                    this.renderUI();
                    this.bindEvents();
                },
                initUI: function () {
                    customerTab.find('.Remarks .value').text(PageEnv.getUserComment());
                },
                bindEvents: function () {
                    var self = this;

                    //显示备注编辑框
                    $('.Remarks .bj').click(function (event) {
                        var valuePanel = $('.Remarks .value');

                        $('.Remarks .word').addClass('on').show(function () {
                            $(this).children('textarea').val(valuePanel.text());
                        });

                        valuePanel.hide();
                        $('.Remarks .bj').hide();
                        $('.Remarks .anniu').show();

                        event.preventDefault();
                        return false;
                    });

                    //备注保存按钮 点击
                    $('.Remarks .pre').click(function (event) {
                        var word = $('.Remarks .word textarea');
                        var val = word.val();

                        if ('' == generalUtil.trim(val)) {
                            alert('请填写备注');
                        } else {
                            $('.Remarks .value').text(val).show();
                            $('.Remarks .word').hide();
                            $('.Remarks .bj').show();
                            $('.Remarks .anniu').hide();

                            window.location.href = "http://im.feiniu.com?comment=" + val;
                        }

                        event.preventDefault();
                        return false;
                    });

                    //备注取消按钮 点击
                    $('.Remarks .cancel').click(function (event) {
                        $('.Remarks .value').show();
                        $('.Remarks .word').hide();
                        $('.Remarks .bj').show();
                        $('.Remarks .anniu').hide();

                        event.preventDefault();
                        return false;
                    });

                    //跳转至订单tab相应子栏
                    $('.Record .lan', customerTab).click(function (event) {
                        $('.mou_width .title .order').click();
                        var $this = $(this);

                        setTimeout(function () {
                            var orderTabHeader = $('.order-tab .head');
                            if ($this.is('.completed')) {
                                orderTabHeader.find('.completed').mouseover();
                            } else if ($this.is('.uncompleted')) {
                                orderTabHeader.find('.uncompleted').mouseover();
                            } else if ($this.is('.cancelled')) {
                                orderTabHeader.find('.cancelled').mouseover();
                            }
                        }, 100);

                        event.preventDefault();
                        return false;
                    });
                    //监听订单摘要更新事件
                    GlobalEventManager.registerEvent(GlobalEventType.RENDER_ORDER_SUMMARY_INFO, function (orderSummaryInfo) {
                        orderRecordPanel.find('.uncompleted').html('(' + orderSummaryInfo.uncompleted + '笔)');
                        orderRecordPanel.find('.completed').html('(' + orderSummaryInfo.completed + '笔)');
                        orderRecordPanel.find('.cancelled').html('(' + orderSummaryInfo.cancelled + '笔)');
                        customerTab.find('.total-trade-info').html('￥' + parseFloat(orderSummaryInfo.totalTradeAmount).toFixed(2)
                            + '（含邮费：￥' + parseFloat(orderSummaryInfo.totalFreight).toFixed(2) + '）');
                    }, this);
                },
                renderUI: function () {
                    if (!hasRenderedBefore && !isOnRetrieveProgress) {
                        isOnRetrieveProgress = true;

                        //请求用户信息
                        (function setupUserInfo() {
                            $.ajax(
                                {
                                    url: PageEnv.getCrpUrlBase(),
                                    type: 'post',
                                    dataType: 'json',
                                    data: {
                                        VENDER_INFO_TYPE: 'GET_BUYER_INFO',
                                        customerToken: PageEnv.getToken(),
                                        serviceId: PageEnv.getServiceId()
                                    },
                                    success: function (resultInfo) {
                                        if (resultInfo.success) {
                                            var consumerTab = $(".content.customer-tab");
                                            var userInfo = resultInfo.data.userInfo;

                                            if (!!userInfo) {
                                                consumerTab.find('.nick-name').html(userInfo.nickname).attr('title', userInfo.nickname);
                                                consumerTab.find('.user-name').html("买家ID：" + userInfo.username);
                                                consumerTab.find('.user-level').html(userInfo.memLevel || 'T0');
                                                consumerTab.find('.user-img').attr('src', userInfo.face_url || 'static/images/moumoupc/icon(1).png');
                                            }
                                        } else {
                                            //alert('获取用户信息失败');
                                        }

                                        isOnRetrieveProgress = false;
                                    },
                                    error: function (XMLHttpRequest, textStatus) {
                                        var errorMessage = '网络发生故障，状态码：' + textStatus;
                                        //alert(errorMessage);
                                        isOnRetrieveProgress = false;
                                    }
                                }
                            );
                        })();

                        //请求 正在咨询商品相关信息
                        (function setupConsultProductInfo() {
                            if (PageEnv.getProductID() != 0) {
                                $.ajax({
                                    url: PageEnv.getUrlBase(),
                                    type: 'post',
                                    dataType: 'json',
                                    data: {
                                        VENDER_INFO_TYPE: 'GET_PRODUCT_INFO',
                                        product_id: PageEnv.getProductID()
                                    },
                                    success: function (result) {
                                        if (result.success) {
                                            var consuPanel = customerTab.find('.content_c>.consultation');

                                            var productInfo = (result.data || {})['goodsAsking'] || {};
                                            productInfo.productURL = PageEnv.getItemUrlBase() + productInfo.skuId;

                                            $('.photo', consuPanel).attr('href', productInfo.productURL);
                                            $('.photo>img', consuPanel).attr('src', productInfo.pic01);
                                            $('.lan', consuPanel).attr('href', productInfo.productURL).html(productInfo.title);
                                            $('.fn-price', consuPanel).html('参考价：￥' + parseFloat(productInfo.marketPrice).toFixed(2));
                                            $('.price', consuPanel).html('优惠价：<b style="font-weight:normal;">￥'
                                                + parseFloat(productInfo.price || productInfo.salePrice || 0).toFixed(2));
                                            $('.num', consuPanel).html('库存：' + productInfo.num
                                                + '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;销量：' + productInfo.sold);

                                            consuPanel.slideDown();
                                        }
                                    }
                                });
                            }
                        })();
                    }
                }
            };
        })();

        //商品标签页管理器
        var ProductManager = (function () {
            var TemplateManager = (function () {
                return '<div class="case border">' +
                    ' <div class="img_black"> ' +
                    '<a href="$url"><img src="$pic" border="0"/></a> ' +
                    '<div class="mask"><a href="$send-link" target="_self">发送链接</a> <p></p></div> ' +
                    '</div> ' +
                    '<div class="with"> ' +
                    '<h1><a class="lan" href="$url">$title</a></h1> ' +
                    '<p class="price">$referPrice</p> <p>优惠价：<b class="fn-price" style="font-weight:normal;">$price</b></p> ' +
                    '<p class="on fl stored">$stored</p> <p class="on fr sold">$sold</p> ' +
                    '</div> ' +
                    '</div>';
            })();

            var PagerManager = (function () {
                function _Pager() {
                    this.productNumPerPage = 10
                    this.currentPageNo = 1;
                    this.isOnRetrieving = false;
                    this.doHasMore = true;
                    this.maxPageNo = 100000;
                }

                _Pager.prototype = {
                    constructor: _Pager,
                    resetConfig: function () {
                        this.currentPageNo = 1;
                        this.doHasMore = true;
                        this.isOnRetrieving = false;
                    },
                    setMaxPageNo: function (maxPageNo) {
                        this.maxPageNo = maxPageNo + 1;
                    },
                    hasMore: function () {
                        return this.doHasMore;
                    },
                    setNoMore: function () {
                        this.doHasMore = false;
                    },
                    canRetrievingNextPage: function () {
                        return this.doHasMore && this.currentPageNo < this.maxPageNo && !this.isOnRetrieving;
                    },
                    getPageNo: function () {
                        return this.currentPageNo;
                    },
                    incrPageNo: function () {
                        this.currentPageNo++;
                        if (null != this.maxPageNo && !isNaN(this.maxPageNo)) {
                            this.doHasMore = this.maxPageNo > this.currentPageNo;
                        }
                    },
                    setStatusToRetrieving: function () {
                        this.isOnRetrieving = true;
                    },
                    resetRetrievingStatus: function () {
                        this.isOnRetrieving = false;
                    },
                    getPageOptions: function () {
                        return {
                            pageRows: this.productNumPerPage,
                            curPage: this.currentPageNo
                        }
                    }
                };

                return {
                    newPager: function () {
                        return new _Pager();
                    }
                }
            })();

            //排序方式
            var SortType = {
                //默认：推荐方式
                RECOMMEND: 'recommend',
                //默认：按销量
                BY_QUANTITY: 'sm_soldqty'
            };

            var ProductService = (function () {
                //获取商品参数
                var DEFAULT_REQUEST_PRODUCT_PARAMS = {
                    VENDER_INFO_TYPE: 'GET_SHOP_GOODS_LIST',
                    sort: SortType.RECOMMEND,
                    cutomerToken: PageEnv.getToken(),
                    serviceId: PageEnv.getServiceId(),
                    merchantId: PageEnv.getMerchantID(),
                    curPage: 1,
                    pageRows: 10
                };

                return {
                    retrieveProductList: function (params, callback) {
                        $.ajax(
                            {
                                url: PageEnv.getCrpUrlBase(),
                                type: 'post',
                                dataType: 'json',
                                data: $.extend(DEFAULT_REQUEST_PRODUCT_PARAMS, params),
                                success: function (resultInfo) {
                                    if ($.isFunction(callback)) {
                                        callback.call(null, resultInfo.data.goodsInfo);
                                    } else {
                                        //alert('获取订单信息失败');
                                    }
                                },
                                error: function (XMLHttpRequest, textStatus) {
                                    var errorMessage = '网络发生故障，状态码：' + textStatus;
                                    //alert(errorMessage);
                                }
                            }
                        );
                    }
                };
            })();

            return {
                init: function () {
                    this.initUI();
                    this.doRetrieveProductList('.product-tab .content_c.hot');
                    this.bindEvents();
                },
                initUI: function () {
                    $('.product-tab .content_c').each(function () {
                        var $this = $(this);
                        var data = {
                            req: {},
                            pager: PagerManager.newPager()
                        };

                        if ($this.is('.hot')) {
                            data.req.sort = SortType.BY_QUANTITY;
                            data.pager.setMaxPageNo(1);
                        } else {
                            data.req.sort = SortType.RECOMMEND;
                        }

                        $this.data(data);
                    });
                },
                doRetrieveProductList: function (selector, needClear) {
                    var panel = $(selector);
                    var req = panel.data('req');
                    var pager = panel.data('pager');
                    var self = this;

                    if (pager.canRetrievingNextPage()) {
                        pager.setStatusToRetrieving();

                        //请求订单列表信息
                        (function setupProductInfo() {
                            ProductService.retrieveProductList($.extend({}, req, pager.getPageOptions()), function (goodsInfo) {
                                //更新翻页信息
                                pager.incrPageNo();
                                pager.resetRetrievingStatus();
                                //如果发现，当前页请求时，已取不到指定数量商品
                                if (goodsInfo.length < pager.getPageOptions().pageRows) {
                                    pager.setNoMore();
                                }

                                //更新商品列表信息
                                self.renderProductList(selector, goodsInfo, needClear);
                            });
                        })();
                    }
                },
                /**
                 * 渲染获取到的产品列表信息
                 * @param selector：产品列表应该插入到的panel的jq选择符
                 * @param productList：产品列表数据
                 * @param needClear：是否要清除掉产品列表之前的产品
                 */
                renderProductList: function (selector, productList, needClear) {
                    var toRenderContent = $(selector);
                    var stringBuilder = '';

                    for (var i = 0; i < productList.length; i++) {
                        var product = productList[i];
                        stringBuilder += TemplateManager.replace(/\$url/g, PageEnv.getItemUrlBase() + product.sm_SEQ)
                            .replace(/\$send-link/g, 'http://im.feiniu.com?link='
                            + generalUtil.encodeUsingBase64(PageEnv.getItemUrlBase() + product.sm_SEQ))
                            .replace(/\$pic/g, product.sm_PIC)
                            .replace(/\$title/g, product.sm_TITLE)
                            .replace(/\$referPrice/g, '参考价：￥' + parseFloat(product.sm_PRICE || 0).toFixed(2))
                            .replace(/\$price/g, parseFloat(product.fnPrice || 0).toFixed(2))
                            .replace(/\$stored/, '库存：' + (product.num || 0))
                            .replace(/\$sold/, '销量：' + (product.sold || 0));
                    }

                    !!needClear && toRenderContent.find('.case').remove();
                    toRenderContent.find('.loading-indicator').remove();
                    toRenderContent.append(stringBuilder);

                    //加滚动提示
                    if (!toRenderContent.is('.hot')) {
                        var hasMore = toRenderContent.data('pager').hasMore();
                        if (hasMore) {
                            toRenderContent.append('<div class="loading-indicator">正在加载....</div>');
                        } else {
                            toRenderContent.append('<div class="loading-indicator">已经到底啦~</div>');
                        }
                    }
                },
                bindEvents: function () {
                    var self = this;
                    var productTab = $('.product-tab');
                    var tabContents = $('.content_c', productTab);

                    (function setupTabEvents() {
                        Tab({
                            "navs": $('.content .clum span'), //点击对象
                            "evens": "mouseover", //事件名
                            "conts": $('.js_tab'), //内容对象
                            "activeClass": 'on', //高亮样式
                            "num": 0, //默认传值
                            callback: function (index) {
                                self.doRetrieveProductList(tabContents.eq(index));
                            }
                        });
                    })();

                    (function setupSearchEvents() {
                        $('.content_c .bd input').click(function () {
                            $(this).addClass('on_1');
                            $('.bd em').removeClass('search').addClass('edit');
                        }).keyup(function (event) {
                            var $this = $(this);
                            var val = $this.val();
                            if (val.length > 100) {
                                $this.val(val.substr(0, 100));
                            }

                            var keyCode = event.keyCode ? event.keyCode : event.which ? event.which : event.charCode;
                            //如果是回车键
                            if (13 == keyCode) {
                                var contentPanel = $this.closest('.content_c');
                                var pager = contentPanel.data('pager');
                                var req = contentPanel.data('req');
                                req.keyWord = $this.val();

                                pager.resetConfig();
                                self.doRetrieveProductList(contentPanel, true);
                            }
                        }).focus(function () {
                            $(this).prev('em').removeClass('search').addClass('edit');
                        }).blur(function () {
                            var $this = $(this);
                            var val = $this.val();

                            if (val == '') {
                                $(this).removeClass('on_1').prev('em').addClass('search').removeClass('edit');
                            }
                        });

                        $("#cha").click(function () {
                            $('#inp_name').val('').focus();
                        });

                    })();

                    (function setupScrollEvents() {
                        /**监听商品栏滚动事件**/
                        tabContents.scroll(function () {
                            var $this = $(this);
                            var pager = $this.data('pager');

                            var scrollHeight = $this.prop('scrollHeight');
                            var height = $this.height();
                            var scrollTop = $this.scrollTop();

                            if (pager.canRetrievingNextPage() && height + scrollTop === scrollHeight) {
                                /**如果滑到滑动条最底部，意味着可以去请求下一页数据了**/
                                self.doRetrieveProductList($this);
                            }
                        });
                    })();

                    (function setupOther() {
                        $(document).on('mouseover', '.img_black', function () {
                            $(this).find('.mask').show();
                        }).on('mouseout', '.img_black', function () {
                            $(this).find('.mask').hide();
                        });
                    })();
                }
            };
        })();

        //订单标签页管理器
        var OrderManager = (function () {
            //请求订单类型
            var ORDER_REQUEST_TYPE = {
                //未完成
                UN_COMPLETED: 1,
                //已完成
                COMPLETED: 2,
                //已取消
                CANCELLED: 3,
                //所有
                ALL: 4
            };
            var OrderStatus = (function () {
                var StatusType = {
                    101: '未付款',
                    102: '待发货',
                    103: '已发货',
                    104: '已完成',
                    105: '已取消'
                };

                return {
                    getStatus: function (orderType, ogStatus) {
                        return StatusType['' + ogStatus];
                    },
                    filterOgStatus: function (filterOrderType, ogStatus) {
                        var orderType = -1;

                        switch (ogStatus) {
                            case 101:
                            case 102:
                            case 103:
                                orderType = ORDER_REQUEST_TYPE.UN_COMPLETED;
                                break;
                            case 104:
                                orderType = ORDER_REQUEST_TYPE.COMPLETED;
                                break;
                            case 105:
                                orderType = ORDER_REQUEST_TYPE.CANCELLED;
                                break;
                        }

                        return orderType == filterOrderType;
                    },
                    //未付款的ogStatus值
                    getValueForUnpay: function () {
                        return "101";
                    },
                    getValueForCancel: function () {
                        return "105";
                    }
                }
            })();

            var OrderServices = (function () {
                //获取订单参数
                var REQUEST_ORDER_PARAMS = {
                    GET_TYPE: 'GET_ORDER_INFO',
                    EDIT_REMARK_ON_ORDER: 'REMARK_ORDER'
                };

                //默认取订单分页参数
                var DefaultPageParams = {
                    curPage: 1,
                    pageRows: 100
                };

                return {
                    retrieveOrderInfo: function (orderType, succCallback, errorCallback) {
                        $.ajax(
                            {
                                url: PageEnv.getCrpUrlBase(),
                                type: 'post',
                                dataType: 'json',
                                data: $.extend({
                                    VENDER_INFO_TYPE: REQUEST_ORDER_PARAMS.GET_TYPE,
                                    customerToken: PageEnv.getToken(),
                                    serviceId: PageEnv.getServiceId(),
                                    orderType: orderType,
                                    merchantId: PageEnv.getMerchantID()
                                }, DefaultPageParams),
                                success: function (resultInfo) {
                                    if (resultInfo.success) {
                                        if ($.isFunction(succCallback)) {
                                            succCallback.call(null, resultInfo.data.orderQueryResult);
                                        }
                                    } else {
                                        if ($.isFunction(errorCallback)) {
                                            errorCallback.call();
                                        }
                                        //alert('获取订单信息失败');
                                    }
                                },
                                error: function (XMLHttpRequest, textStatus) {
                                    var errorMessage = '网络发生故障，状态码：' + textStatus;
                                    //alert(errorMessage);
                                }
                            }
                        );
                    },
                    //修改商家对包裹的备注
                    saveMerchantRemarkOnOrder: function (params, callback) {
                        $.ajax(
                            {
                                url: PageEnv.getCrpUrlBase(),
                                type: 'post',
                                dataType: 'json',
                                data: $.extend({
                                    VENDER_INFO_TYPE: REQUEST_ORDER_PARAMS.EDIT_REMARK_ON_ORDER,
                                    customerToken: PageEnv.getToken(),
                                    serviceId: PageEnv.getServiceId(),
                                    modUsr: PageEnv.getServiceId()
                                }, params),
                                success: function (resultInfo) {
                                    if (resultInfo.success) {
                                        if ($.isFunction(callback)) {
                                            callback.call();
                                        }
                                    } else {
                                        //alert('获取订单信息失败');
                                    }
                                },
                                error: function (XMLHttpRequest, textStatus) {
                                    var errorMessage = '网络发生故障，状态码：' + textStatus;
                                    //alert(errorMessage);
                                }
                            });
                    }
                }
            })();

            var OrderTemplate = (function () {
                return '<div class="case_order"> ' +
                    '<div class="top"> <i></i> <p>$orderStatus</p> <em>$orderDate</em> </div> ' +
                    '<div class="all"></div>';
            })();
            var ProductTemplate = (function () {
                return '<div class="case"> ' +
                    '<img src="$productImgUrl"> ' +
                    '<div class="bt"> ' +
                    '<p><a href="$productUrl" class="lan">$productTitle</a></p> ' +
                    '<span class="fl">$productProp</span> <span class="fr t_r">￥$productPrice</span> ' +
                    '</div> ' +
                    '</div> ';
            })();
            var OrderSummaryTemplate = (function () {
                return '<ul> ' +
                    '<li>订单金额：<b>￥$orderPrice</b>（含邮费：￥$freightPrice）</li> ' +
                    '<li>订单编号：$orderSeq</li> ' +
                    '<li class="order-date hide">付款时间：$payDate</li>' +
                    '</ul>';
            })();
            var OrderControlTemplate = (function () {
                return '<div class="ui-operation-bar" js-hook="operation-bar"> ' +
                    '<ul class="operation-nav"> ' +
                    '<li data-index="0" class="operation-nav-item">备注</li>' +
                    '<li data-index="1" class="operation-nav-item">买家备注</li>' +
                    '<li data-index="2" class="operation-nav-item">地址</li>' +
                    '<li data-index="3" class="operation-nav-item prompt-pay" style="display: none;">催付</li> ' +
                    '<li class="delivery" style="display: none"><a href="$deliveryUrl" class="link-skip">发货</a></li> ' +
                    '</ul> ' +
                    '<ul class="operation-panel"> ' +
                    '<li data-index="0" class="operation-panel-item" data-index="0"> <!-- js-修改面板功能 --> ' +
                    '<div class="amend-panel" js-hook="amend"> ' +
                    '<div class="js-edit-panel"> <div class="bd"> <div class="ui-edit-txt"> ' +
                    '备注：<a href="##">$merchantRemark</a> ' +
                    '</div> ' +
                    '</div> ' +
                    '<div class="ft"> <a href="##" class="ui-clickable js-edit">编辑</a> </div> ' +
                    '</div> ' +
                    '<div style="display:none;" class="js-save-panel"> ' +
                    '<div class="bd"> <input class="ui-type-in"></div> ' +
                    '<div class="ft"> <a href="##" class="ui-clickable js-save save-merchant-remark">保存</a>' +
                    ' <a href="##" class="ui-clickable js-cancel">取消</a> </div> ' +
                    '</div> </div> ' +
                    '</li> ' +
                    '<li data-index="1" class="operation-panel-item" data-index="1" > ' +
                    '<div class="ui-edit-txt"><a href="##">备注：$buyerRemark</a></div> </li> ' +
                    '<li data-index="2" class="operation-panel-item address-operation-panel" data-index="2" >' +
                    '<div class="ui-edit-txt"> <a href="##" title="点击复制到发送框">收货人：$memberName<br>' +
                    '地址：$memberAddress<br>' +
                    '邮编：$memberPostCode<br>电话：$memberCellphone<br>' +
                    '座机：$memberTel</a> </div> </li> ' +
                    '<li data-index="3" class="operation-panel-item" data-index="3" > <!-- js-修改面板功能 --> ' +
                    '<div class="amend-panel " js-hook="amend"> <div class="js-edit-panel"> ' +
                    '<div class="bd"> <div class="ui-edit-txt"> ' +
                    '<a href="##">亲，您在本店购买的宝贝还没有付款哦， 付款后我们会尽快为您安排发货哦～</a> </div> </div> ' +
                    '<div class="ft"> <a href="##" class="ui-clickable js-edit">编辑</a> </div> </div> ' +
                    '<div style="display:none;" class="js-save-panel"> <div class="bd"> ' +
                    '<input contenteditable="true" class="ui-type-in"></div> <div class="ft"> ' +
                    '<a href="##" class="ui-clickable js-save">保存</a> <a href="##" class="ui-clickable js-cancel">取消</a> </div> </div> </div> ' +
                    '<!-- js-修改面板功能 --> ' +
                    '<div class="amend-panel" js-hook="amend"> ' +
                    '<div class="js-edit-panel"> <div class="bd"> <div class="ui-edit-txt"> ' +
                    '<a href="##">亲，您在本店购买的宝贝还没有付款哦， 付款后我们会尽快为您安排发货哦～</a> </div> </div> ' +
                    '<div class="ft"> <a href="##" class="ui-clickable js-edit">编辑</a> </div> </div> ' +
                    '<div style="display:none;" class="js-save-panel"> <div class="bd"> ' +
                    '<div contenteditable="true" class="ui-type-in"></div> </div> ' +
                    '<div class="ft"> <a href="##" class="ui-clickable js-save">保存</a> <a href="##" class="ui-clickable js-cancel">取消</a> </div> </div> </div> ' +
                    '</li> </ul> ' +
                    '</div>';
            })();

            var OrderViewer = (function () {
                return {
                    renderOrderList: function (selector, orderList, orderType) {
                        var toRenderContent = $(selector);
                        toRenderContent.empty();

                        if (orderList.length == 0) {
                            toRenderContent.append('<div class="loading-indicator" style="position: absolute;top: 45%;width: 100%">Ta还没有订单哦~~</div>');
                            return;
                        }

                        for (var i = 0; i < orderList.length; i++) {
                            var order = orderList[i];
                            if (!OrderStatus.filterOgStatus(orderType, order.ogsStatus)) {
                                continue;
                            }

                            order.insDt *= 1000;
                            var orderDate = generalUtil.isBeforeToday(order.insDt)
                                ? generalUtil.formatTime(order.insDt)
                                : generalUtil.formatOnlyTime(order.insDt);

                            var orderPanel = $(
                                OrderTemplate.replace(/\$orderStatus/, OrderStatus.getStatus(orderType, order.ogsStatus))
                                    .replace(/\$orderDate/, orderDate)
                            );
                            var orderPanelData = {
                                buyerRemark: (order['memMsg'] || '').replace(/[<]/g, '&lt').replace(/[>]/g, '&gt'),
                                merchantRemark: (order['merchantRemark'] || '').replace(/[<]/g, '&lt').replace(/[>]/g, '&gt'),
                                memberName: order['memberName'] || '',
                                memberAddress: order['memberAdd'] || '',
                                memberCity: order['memberCity'] || '',
                                memberPostArea: order['memberPostArea'] || '',
                                memberProvince: order['memberProvince'] || '',
                                memberPostCode: order['memberPostCode'] || '',
                                memberCellphone: order['memberCellphone'] || '',
                                memberTel: order['memberTel'] || '',
                                orderNo: order.orderNo,
                                ogsSeq: order.ogsSeq
                            };
                            orderPanel.data(orderPanelData);

                            var orderContentPanel = orderPanel.find('.all');

                            //添加订单里面的商品信息
                            var goodsList = order.goodsList || [];
                            for (var j = 0; j < goodsList.length; j++) {
                                var product = goodsList[j];

                                orderContentPanel.append(
                                    ProductTemplate.replace(/\$productImgUrl/, product.productMainUrl)
                                        .replace(/\$productUrl/, PageEnv.getItemUrlBase() + product.skuId)
                                        .replace(/\$productTitle/, product.productName)
                                        .replace(/\$productProp/, '[&nbsp;' + product.colorprop + '&nbsp;' + product.salerprop +
                                        '&nbsp;' + product.productQty + '件' + '&nbsp;]')
                                        .replace(/\$productPrice/, parseFloat(product.productPrice).toFixed(2))
                                );
                            }

                            //订单摘要信息
                            var orderSummary = $(OrderSummaryTemplate
                                    .replace(/\$orderPrice/g, parseFloat(order.totalPrice).toFixed(2))
                                    .replace(/\$freightPrice/g, parseFloat(order.freight).toFixed(2))
                                    .replace(/\$orderSeq/g, order.orderNo)
                                    .replace(/\$payDate/g, generalUtil.formatTime(order.payDt * 1000))
                            );
                            if (OrderStatus.getValueForUnpay() != order.ogsStatus
                                && OrderStatus.getValueForCancel() != order.ogsStatus) {
                                orderSummary.find('.order-date').removeClass('hide');
                            }

                            //订单操作信息
                            var orderControl = $(OrderControlTemplate
                                    .replace(/\$merchantRemark/g, orderPanelData.merchantRemark)
                                    .replace(/\$buyerRemark/g, orderPanelData.buyerRemark)
                                    .replace(/\$memberName/g, orderPanelData.memberName)
                                    .replace(/\$memberAddress/g, orderPanelData.memberProvince + '-' + orderPanelData.memberCity
                                    + '-' + orderPanelData.memberPostArea + '-' + orderPanelData.memberAddress)
                                    .replace(/\$memberPostCode/g, orderPanelData.memberPostCode)
                                    .replace(/\$memberTel/g, orderPanelData.memberTel)
                                    .replace(/\$memberCellphone/g, orderPanelData.memberCellphone)
                                    .replace(/\$deliveryUrl/g, PageEnv.getDeliveryUrlBase() + 'invoice/order_delivery.do?mode=1&orderIds=' + orderPanelData.ogsSeq)
                            );

                            orderContentPanel.append(orderSummary);
                            orderPanel.append(orderControl);
                            toRenderContent.append(orderPanel);

                            if (orderType == ORDER_REQUEST_TYPE.UN_COMPLETED) {
                                if (OrderStatus.getValueForUnpay() != order.ogsStatus) {
                                    orderControl.find('.delivery').css('display', 'block');
                                }
                            }
                        }
                    }
                };
            })();

            return {
                init: function () {
                    this.initUI();
                    this.renderUI('.order-tab .complete.uncompleted');
                    this.bindEvents();
                },
                initUI: function () {
                    $('.order-tab .complete').each(function (index) {
                        var $this = $(this);
                        var orderType = -1;

                        if ($this.is('.uncompleted')) {
                            orderType = ORDER_REQUEST_TYPE.ALL;
                        } else if ($this.is('.completed')) {
                            orderType = ORDER_REQUEST_TYPE.COMPLETED;
                        } else if ($this.is('.cancelled')) {
                            orderType = ORDER_REQUEST_TYPE.CANCELLED;
                        }

                        $this.data('orderType', orderType);
                    });
                },
                renderUI: function (selector) {
                    var panel = $(selector);
                    var hasRenderedBefore = panel.data('hasRenderedBefore');
                    var isRetrieveOnProgress = panel.data('isRetrieveOnProgress');
                    var orderType = panel.data('orderType');

                    if (orderType > 0 && !hasRenderedBefore && !isRetrieveOnProgress) {
                        panel.data('isRetrieveOnProgress', true);

                        //请求订单列表信息
                        (function setupOrderInfo() {
                            OrderServices.retrieveOrderInfo(orderType, function (orderInfo) {
                                //如果查询所有订单，更新订单摘要信息
                                if (orderType == ORDER_REQUEST_TYPE.ALL) {
                                    GlobalEventManager.dispatchEvent(GlobalEventType.RENDER_ORDER_SUMMARY_INFO, {
                                        uncompleted: orderInfo.noPay + orderInfo.noDelivery + orderInfo.delivery,
                                        completed: orderInfo.allFinish,
                                        cancelled: orderInfo.cancelled,
                                        totalTradeAmount: orderInfo.allPrice || 0,
                                        totalFreight: orderInfo.allFreight || 0
                                    });

                                    orderType = ORDER_REQUEST_TYPE.UN_COMPLETED;
                                }

                                //更新订单列表信息
                                OrderViewer.renderOrderList(selector, orderInfo.orderMallList, orderType);

                                panel.data('hasRenderedBefore', true);
                                panel.data('isRetrieveOnProgress', false);
                            }, function () {
                                $(selector).empty();

                                panel.data('hasRenderedBefore', false);
                                panel.data('isRetrieveOnProgress', false);
                            });
                        })();
                    }
                },
                bindEvents: function () {
                    var self = this;
                    var navTabHeaders = $('.content .head span');
                    var orderContentList = $('.order-tab .complete');

                    (function setupNavLiEvents() {
                        $('.state li').click(function () {
                            $(this).toggleClass('on_bg').siblings().removeClass('on_bg');
                        });

                        $('.state li').click(function () {
                            $('.note .bj').click(function () {
                                $('.note .beizhu').hide();
                                $('.note .word').show();
                                $('.note .bc').click(function () {
                                    $('.note').show();
                                    $('.note .beizhu').show();
                                    $('.note .word').hide();

                                })
                            })
                        });

                        $('.state .bz').click(function () {
                            $('.state .note').toggle();
                            $('.state .buyer').hide();
                            $('.state .address').hide();
                            $('.state .pay').hide();
                        });

                        $('.state .buy').click(function () {
                            $('.state .buyer').toggle();
                            $('.state .note').hide();
                            $('.state .address').hide();
                            $('.state .pay').hide();
                        });

                        $('.state .dz').click(function () {
                            $('.state .address').toggle();
                            $('.state .buyer').hide();
                            $('.state .note').hide();
                            $('.state .pay').hide();
                        });

                        $('.state .cf').click(function () {
                            $('.state .pay').toggle();
                            $('.state .buyer').hide();
                            $('.state .note').hide();
                            $('.state .address').hide();

                        });

                        //切换tab页
                        Tab({
                            "navs": $('.content .head span'), //点击对象
                            "evens": "mouseover", //事件名
                            "conts": $('.complete'), //内容对象
                            "activeClass": 'on', //高亮样式
                            "num": 0, //默认传值
                            callback: function (index) {
                                self.renderUI(orderContentList.eq(index));
                            }
                        });
                    })();

                    (function setupEdit() {
                        //选择编辑项
                        $(document).on('click', '.operation-nav-item', function () {
                            var $this = $(this);
                            var num = $this.attr('data-index');
                            var parent = $this.closest('.operation-nav');
                            var panelItems = parent.siblings('.operation-panel').children('.operation-panel-item');

                            $this.toggleClass('cur').siblings().removeClass('cur');
                            panelItems.filter('[data-index=' + num + ']').toggleClass('cur');
                            panelItems.filter(':not([data-index=' + num + '])').removeClass('cur');
                        });

                        //编辑
                        $(document).on('click', '.operation-panel .js-edit', function (event) {
                            var $this = $(this);
                            var editPanel = $this.closest('.js-edit-panel');
                            var savePanel = editPanel.siblings('.js-save-panel');
                            var sEdit = editPanel.find('.ui-edit-txt a').text();

                            savePanel.find('.ui-type-in').val(sEdit);

                            editPanel.hide();
                            savePanel.show();

                            event.preventDefault();
                            return false;
                        });

                        //保存
                        $(document).on('click', '.operation-panel .js-save', function (event) {
                            var $this = $(this);
                            var savePanel = $this.closest('.js-save-panel');
                            var editPanel = savePanel.siblings('.js-edit-panel');
                            var oldValueInput = editPanel.find('.ui-edit-txt a');
                            var newValue = savePanel.find('.ui-type-in').val();
                            newValue = newValue.replace(/[<]/g, '&lt').replace(/[>]/g, '&gt');
                            var orderPanel = savePanel.closest('.case_order');
                            var ogsSeq = orderPanel.data('ogsSeq');

                            if (newValue == '') {
                                alert('备注不能为空');
                                savePanel.find('.ui-type-in').focus();
                            } else {
                                if (oldValueInput.html() != newValue && !!ogsSeq) {
                                    OrderServices.saveMerchantRemarkOnOrder({
                                        ogsSeqs: ogsSeq,
                                        remark: newValue
                                    }, function () {
                                        oldValueInput.html(newValue);

                                        savePanel.hide();
                                        editPanel.show();
                                    });
                                } else {
                                    savePanel.hide();
                                    editPanel.show();
                                }

                            }

                            event.preventDefault();
                            return false;
                        });

                        //取消
                        $(document).on('click', '.operation-panel .js-cancel', function (event) {
                            var $this = $(this);
                            var savePanel = $this.closest('.js-save-panel');
                            var editPanel = savePanel.siblings('.js-edit-panel');
                            var sEdit = savePanel.find('.ui-type-in').val();

                            editPanel.find('.ui-type-in').val(sEdit);

                            savePanel.hide();
                            editPanel.show();

                            event.preventDefault();
                            return false;
                        });

                        //发送地址
                        $(document).on('click', '.operation-panel .operation-panel-item.address-operation-panel .ui-edit-txt a', function (e) {
                            window.location.href = 'http://im.feiniu.com?link='
                                + generalUtil.encodeUsingBase64('请确认您的地址是否正确？\r\n' + $(this).html().replace(/<br>/g, '\r\n'));

                            e.preventDefault();
                            return false;
                        });
                    })();

                    (function setupCaseOrderEvents() {
                        $(document).on('click', '.complete .case_order .top', function () {
                            $(this).find('i').toggleClass('on');
                            $(this).nextAll().slideToggle();
                        });
                    })();

                    (function setupRefreshBtn() {
                        $('#refreshCurrentTabOrder').click(function () {
                            var currentActiveTab = navTabHeaders.filter('.on');
                            var index = currentActiveTab.index();

                            var orderContent = orderContentList.eq(index);
                            orderContent.data('hasRenderedBefore', false);
                            orderContent.data('isRetrieveOnProgress', false);

                            self.renderUI(orderContent);
                        });
                    })();
                }
            }
        })();

        return {
            init: function () {
                this.initComponent();
                this.initUI();
                this.bindEvents();
            },
            initComponent: function () {
                if (PageEnv.checkIsOppositeCustomer()) {
                    $('.mou_width>.title').find('.customer-header,.order-header').show();

                    CustomerManager.init();
                    OrderManager.init();
                } else {
                    $('.mou_width>.title>.product-header').addClass('on');
                    $('.mou_width').find('.customer-tab').hide();
                    $('.mou_width').find('.product-tab').show();
                }

                ProductManager.init();
            },
            initUI: function () {
            },
            bindEvents: function () {
                (function bindTabEvents() {
                    Tab({
                        "navs": $('.title span'), //点击对象
                        "evens": "click", //事件名
                        "conts": $('.content'), //内容对象
                        "activeClass": 'on', //高亮样式
                        "num": 0 //默认传值
                    });
                })();
            }
        };
    })();

    (function bootstrap() {
        $(function () {
            IMSeller.init();
        });
    })();
});
