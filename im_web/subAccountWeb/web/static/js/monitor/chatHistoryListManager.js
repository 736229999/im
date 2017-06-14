/**
 * Created by wangchen on 2015/2/5.
 */
var chatHistoryListManager = {
    _pager: null,
    _subMerchantIDList: [],
    _emotionPath: '',

    init: function (options) {
        this.initUI();
        this.bindUI();
        this.initData(options);
    },
    initData: function (options) {
        var currentHref = window.location.href;
        var currentDomain = currentHref.replace(new RegExp('^https?://([^/]+)/.+$'), '$1');

        if (currentDomain.indexOf('.fn') > -1 || currentDomain.indexOf('.fn.com') > -1) {
            this._emotionPath = 'http://webim.beta1.fn/webim/';
        } else {
            this._emotionPath = 'http://merchant.feiniu.com/webim/';
        }

        var self = this;

        self._pager = options.pager;

        self.retrieveHistoryMessageList(false);
    },
    initUI: function () {
        $('#endTime').val(generalUtil.defaultDate());
        $('#startTime').val(generalUtil.defaultDate());
    },
    bindUI: function () {
        var self = this;

        $('#subMerchantIDPanel .commonSelect ul li').click(function () {
            var $this = $(this);

            $('#currentSubMerchantInfoPanel .subMemberName').html($(this).text());
            $this.addClass('active').siblings().removeClass('active');
        });

        $('#startTime').click(function () {
            WdatePicker({dateFmt: 'yyyy-MM-dd', minDate: '%y-#{%M-3}-%d', maxDate: '%y-%M-#\{%d\}'});
        });

        $('#endTime').click(function () {
            WdatePicker({dateFmt: 'yyyy-MM-dd', minDate: '%y-#{%M-3}-%d', maxDate: '%y-%M-#\{%d\}'});
        });

        $('#search').on('click', function () {
            self.retrieveHistoryMessageList(false);
        });
    },
    constructMessagePanel: function (params, result) {
        $('#historyMessageListPanel').empty();
        var self = this;
        var historyMessageList = result.data.pager.resultsForCurrentPage;
        var historyMessageContent = '';
        var dayParagragh = '';
        var lastDayMessageArray = [];
        var subMerchantIDList = (params.subMerchantIDList + '').split(";");

        if (!!historyMessageList) {
            for (var i = 0; i < historyMessageList.length; i++) {
                var currentMessage = historyMessageList[i];

                var date = new Date();
                date.setTime(currentMessage.sendTime * 1000);
                var day = generalUtil.formateDate(date);

                if (dayParagragh != day) {
                    dayParagragh = day;
                    historyMessageContent += lastDayMessageArray.join('');
                    historyMessageContent += '<p style="font-weight: bold;">' + dayParagragh + '</p>';
                    lastDayMessageArray = [];
                }

                var messageHeader = currentMessage.fromUser.nickName
                    + '('
                    + generalUtil.formateOnlyTime(date)
                    + ')';

                if (-1 != $.inArray(currentMessage.fromUser.userID + '', subMerchantIDList)) {
                    messageHeader = '<span style="color: #008040">' + messageHeader + '</span>';
                } else {
                    messageHeader = '<span style="color: #407ea6">' + messageHeader + '</span>';
                }

                var messageBody = currentMessage.msg.replace(/<image:([^>]+)>/g, function () {
                    var $1 = arguments[1] || '';
                    if ($1.indexOf('http://') != -1) {
                        return '<img src="' + $1 + '">';
                    } else {
                        if($1.length < 12)
                            return '<a class="fn-fancy-item" href="' + self._emotionPath + 'static/images/feiniu-emotion/' + $1 +
                                '"><img src="' + self._emotionPath + 'static/images/feiniu-emotion/' + $1 + '" alt="聊天图片"/></a>';
                        else {
                            var url = feiniu.image.getUrl($1);
                            return '<a class="fn-fancy-item" href="' + url +
                                '"><img src="' + url + '" alt="聊天图片"/></a>';
                        }
                    }
                }).replace(/\r/g, '<br/>');

                lastDayMessageArray.unshift('<p>' + messageHeader + '</p><p>' + messageBody + '</p>');
            }

            historyMessageContent += lastDayMessageArray.join('');
        }

        $('#historyMessageListPanel').append(historyMessageContent);
    },
    constructQueryParams: function () {
        var self = this;
        var params = {
            subMerchantIDList: '',
            customerName: '',
        };

        params.subMerchantIDList = $("#subMerchantID").val();
        var customerName = $('#customerName').val();
        !!customerName && (params['customerName'] = customerName);

        var startTime = $('#startTime').val();
        var endTime = $('#endTime').val();
        !!startTime && (params['startTime'] = generalUtil.parseDate(startTime).getTime() / 1000);
        !!endTime && (params['endTime'] = generalUtil.parseDate(endTime).getTime() / 1000 + 24 * 3600);

        $.extend(params, self._pager.getPagerOption());
        params['currentPageNo'] = 1;

        return params;
    },
    checkQueryConditions: function () {
        var startTime = $('#startTime').val();
        var endTime = $('#endTime').val();
        var gap = (new Date(endTime) - new Date(startTime)) / (24 * 3600 * 1000);

        if (!startTime) {
            alert('请选择开始时间');
            return false;
        }
        if (!endTime) {
            alert('请选择结束时间');
            return false;
        }
        if (gap < 0 || gap > 7) {
            alert('结束时间和开始时间之间的间隔天数不能超过7天');
            return false;
        }

        return true;
    },
    retrieveHistoryMessageList: function (isFromPager, paramsFromPager) {
        var self = this;

        if (!self.checkQueryConditions()) {
            return;
        }

        var params = self.constructQueryParams();
        params = isFromPager ? paramsFromPager : params;

        $.post(
            '../monitor/gethistorymessagelist',
            params,
            function (result) {
                if (!result || !result.success) {
                    alert(result.message);
                    return;
                } else {
                    self.constructMessagePanel(params, result);

                    self._pager.updatePager($.extend(result.data.pager, {
                        retrieveFn: function (params) {
                            self.retrieveHistoryMessageList.call(self, true, params);
                        }
                    }, {
                        requestParams: params
                    }));
                }
            },
            'json'
        );
    }
};

var pager = (function () {
    var DefaultPagerOption = {
        currentPageNo: 1,
        pageSize: 5,
        totalPageNum: -1,
        retrieveFn: function () {
        },
        _updateOption: function (pageOption) {
            $.extend(this, pageOption);
        },
        exportOptions: function () {
            var options = $.extend({}, this.requestParams);

            return options;
        }
    };

    return {
        _btnTemplate: '<a data-pagerno= "$pagerno" class="pager $pagerclass">$name</a>',
        _pagerSpecialLeftBtnClass: 'pageLeftBg',
        _pagerSpecialRightBtnClass: 'pageRightBg',

        getPagerOption: function () {
            return {
                currentPageNo: DefaultPagerOption.currentPageNo,
                pageSize: DefaultPagerOption.pageSize
            }
        },
        init: function () {
            this.bindEvent();
        },
        updatePager: function (pageEnv) {
            DefaultPagerOption._updateOption(pageEnv);

            var pagerPanel = $('.employeepage');
            pagerPanel.empty();
            var pagerList = [];

            var hasPrevious = pageEnv.currentPageNo > 1;
            hasPrevious && pagerList.push(this.constructPagerElement({
                pagerno: pageEnv.currentPageNo - 1,
                pagerclass: this._pagerSpecialRightBtnClass + ' previous-page',
                name: '上一页'
            }));

            var previousIndex = parseInt(pageEnv.currentPageNo - pageEnv.pageSize / 2 + '');
            previousIndex = previousIndex < 1 ? 1 : previousIndex;
            for (var i = previousIndex; i < pageEnv.currentPageNo; i++) {
                pagerList.push(this.constructPagerElement({
                    pagerno: i,
                    pagerclass: '',
                    name: i
                }));
            }
            var nextIndex = parseInt(pageEnv.currentPageNo + pageEnv.pageSize / 2 + '');
            nextIndex = nextIndex > pageEnv.totalPageNum ? pageEnv.totalPageNum : nextIndex;
            for (; i <= nextIndex; i++) {
                pagerList.push(this.constructPagerElement({
                    pagerno: i,
                    pagerclass: i == pageEnv.currentPageNo ? 'active' : '',
                    name: i
                }));
            }

            var hasNext = pageEnv.totalPageNum > pageEnv.currentPageNo;
            hasNext && pagerList.push(this.constructPagerElement({
                pagerno: pageEnv.currentPageNo + 1,
                pagerclass: this._pagerSpecialRightBtnClass + ' next-page',
                name: '下一页'
            }));

            pagerPanel.append(pagerList.join(''));
            pagerPanel.children('a').removeClass('active');
            pagerPanel.children('a[data-pagerno="' + pageEnv.currentPageNo + '"]').addClass('active');
        },
        bindEvent: function () {
            $(document).on('click', '.employeepage a.pager', function () {
                DefaultPagerOption.retrieveFn.call(null, $.extend({},
                    DefaultPagerOption.exportOptions(), {
                        currentPageNo: $(this).data('pagerno')
                    }));
            });
        },

        appendNewPage: function () {

        },
        constructPagerElement: function (option) {
            return this._btnTemplate.replace(/\$\w+/g, function (result) {
                return option[result.replace(/\$/, '')];
            });
        }
    };
}());

$(function () {
    pager.init();
    chatHistoryListManager.init({
        pager: pager
    });
});
