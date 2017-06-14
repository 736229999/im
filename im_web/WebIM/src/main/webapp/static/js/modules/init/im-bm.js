/**
 * Created by chenqing on 2016/2/18.
 */
seajs.use(['modules/feiniu', 'modules/util/generalServiceHelper', 'modules/util/generalUtil', 'JSON'],
    function (feiniu, serviceHelper, generalUtil, JSON) {
        var params = {
            objMerchantId: 71, //商家主Id
            objSubMerchantId: 10041 //商家子账号id
        };

        function getUrlParam(name) {
            var reg = new RegExp("(^|&)" + name + "=([^&]*)(&|$)"); //构造一个含有目标参数的正则表达式对象
            var r = window.location.search.substr(1).match(reg);  //匹配目标参数
            if (r != null) return unescape(r[2]);
            return null; //返回参数值
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
                    this.storeUrlBase = 'http://storefront.beta1.fn/index-';
                } else {
                    this.storeUrlBase = 'http://store.feiniu.com/index-';
                }
            };

            config.prototype = {
                constructor: config,
                getStoreUrlBase: function () {
                    return this.storeUrlBase;
                }
            };

            return new config();
        })();

        var im_bm = {
            init: function () {
                this.getparams();

            },

            config: {
                SERVER: 'vender'
            },

            /**
             * 获取参数
             */
            getparams: function () {
                params.objMerchantId = getUrlParam("merchantId");
                if (generalUtil.isNumber(params.objMerchantId)) {
                    this.getMerchantInfo();
                    this.getCommentInfo();
                    this.getStoreInfo();
                } else {
                    params.objSubMerchantId = getUrlParam("submerchantId");
                    if (generalUtil.isNumber(params.objSubMerchantId)) {
                        var self = this;
                        $.post(self.config.SERVER,
                            {
                                VENDER_INFO_TYPE: 'GET_MERCHANT_IDS_BY_MEMBER_IDS',
                                sub_merchant_id: params.objSubMerchantId
                            },
                            function (result) {
                                if (result.success) {
                                    params.objMerchantId = (result.data || {})['merchantID'] || {};
                                    self.getMerchantInfo();
                                    self.getCommentInfo();
                                    self.getStoreInfo();
                                } else {
                                    $(document).find('#J_merchant_name').html("输入参数有误,获取商家账号失败！");
                                }
                            },
                            'json'
                        );
                    } else {

                        $(document).find('#J_merchant_name').html("输入参数有误,获取商家信息失败！");
                    }
                }


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
                    function (result) {
                        if (result.success) {
                            var merchantInfo = (result.data || {})['merchantInfo'] || {};
                            $(document).find('#J_merchant_name').html(merchantInfo.merchantName);
                            !!merchantInfo.merchantId && (params.objMerchantId = merchantInfo.merchantId);
                        } else {
                            $(document).find('#J_merchant_name').html("获取商家信息失败");
                        }
                    },
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
                    function (result) {
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
                    function (result) {
                        if (result.success) {
                            var storeName = (result.data || {})['storeName'];
                            storeName = storeName == null ? '' : storeName;

                            if (storeName) {
                                window.parent.document.title = storeName;
                                window.parent.refreshTitleHandler && window.parent.refreshTitleHandler(storeName);

                                $(window.parent.document).find('#J_store_name').html(storeName);
                                $('#J_store_entrance').attr('href', PageEnv.getStoreUrlBase() + storeName + '.html');
                            } else {
                                $(window.parent.document).find('#J_store_name').html("获取店铺信息失败~");
                            }
                        } else {
                            $(window.parent.document).find('#J_store_name').html("获取店铺信息失败~");
                        }
                    },
                    'json'
                );
            }
        };


        (function () {
            im_bm.init.call(im_bm);
        })();

    });