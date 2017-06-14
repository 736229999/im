function LeftTree($container, options, leftOptions) {
    var _me = this;
    var opt = $.extend({}, {
        idFiled: 'id',
        parentField: 'pid',
        textFiled: 'name',
        data: []
    }, options || {});

    var leftOpt = $.extend({}, {
        onAdd: function (data) {
            data.callback({flag: false});
        },
        onEdit: function (data) {
            data.callback({flag: false});
        },
        onRemove: function (data) {
            data.callback({flag: false});
        }
    }, leftOptions || {});


    var tree = null;
    var opSuffix = 'op';
    var penel = null;


    function formatNode(node) {
        var textFiled = opt.textFiled || 'text';
        var html = '<span class="node-text">' + node[textFiled] + '</span><a id="' + node.id + opSuffix + '" class="op-bar" href="javascript:void(0);"></a>';
        node.text = html;
        return node;
    }

    function formatData(data) {
        var idFiled, parentField;
        if (!opt.parentField)return data;

        idFiled = opt.idFiled || 'id';
        parentField = opt.parentField;
        var i, l, treeData = [], tmpMap = [];
        for (i = 0, l = data.length; i < l; i++) {
            tmpMap[data[i][idFiled]] = data[i];
        }
        for (i = 0, l = data.length; i < l; i++) {
            if (tmpMap[data[i][parentField]] && data[i][idFiled] != data[i][parentField]) {
                if (!tmpMap[data[i][parentField]]['children'])
                    tmpMap[data[i][parentField]]['children'] = [];
                formatNode(data[i]);
                tmpMap[data[i][parentField]]['children'].push(data[i]);
            } else {
                formatNode(data[i]);
                treeData.push(data[i]);
            }
        }
        return treeData;
    };

    function createInputBox($text, blurFun) {
        //input新建
        return $('<input type="text" class="rename" value="' + $text.text() + '"/>').css({
            top: $text.offset().top - 5,
            left: $text.offset().left
        }).appendTo($container).on('blur', function (event) {
            var _this = this;
            setTimeout(function () {
                blurFun.call(_this, event);
            }, 0);
        });
    };

    _me.addNode = function (node, callbacks, properties) {
        //addFun.call(this, arguments);
        addFun(node, callbacks, properties);
    };

    function addFun(node, callbacks, properties) {
        //取消动画效果
        tree.tree('options').animate = false;
        //产生临时节点ID
        var uuid = new Date().getTime();

        //产生临时节点
        var tmpNode = formatNode({
            id: uuid,
            name: properties['lastName'] || '新部门'
        });

        if (!node) {
            var root = tree.tree('getRoot');
            //如果有根节点，则在根节点后面新增
            if (root) {
                tree.tree('append', {
                    after: root.target,
                    data: [tmpNode]
                });
                //如果没有根节点，在根节点新增
            } else {
                tree.tree('append', {
                    data: [tmpNode]
                });
            }
        } else {
            tree.tree('append', {
                parent: node.target,
                data: [tmpNode]
            });
        }

        //找到渲染后的节点
        tmpNode = tree.tree('find', uuid);
        //展开渲染后的节点
        tree.tree('expandTo', tmpNode.target);

        //创建输入框
        var $input = createInputBox($('.node-text', tmpNode.target), function () {
            //调用回调
            leftOpt.onAdd.call(tree[0], {
                name: $input.val(),
                node: node,
                callback: function (dd) {
                    var data = dd || {};

                    if (data.flag) {
                        tree.tree('insert', {
                            before: tmpNode.target,
                            data: formatNode(data.node)
                        });
                    }

                    tree.tree('remove', tmpNode.target);
                    tree.tree('options').animate = true;

                    renderHandler();
                    $input.remove();

                    try {
                        if (!!callbacks && typeof callbacks['onAdd'] === 'function') {
                            properties['lastName'] = $input.val();
                            properties['groupName'] = properties['lastName'];
                            callbacks['onAdd'].call(null, properties);
                        }
                        $(data.node.target).
                            data('callbacks', callbacks).
                            data('properties', properties).
                            trigger('click');
                    } catch (ex) {
                        tree.tree('remove', data.node.target);
                        alert(ex.message);
                    }
                    ;
                }
            });
        });

        $input.click(function () {
            $(this).select();
        }).trigger('click');
    };

    function editFun(node) {
        var $text = $('.node-text', node.target);
        var $input = createInputBox($text, function () {
            //调用回调
            leftOpt.onEdit.call(tree[0], {
                name: $input.val(),
                node: node,
                callback: function (dd) {
                    var data = dd || {};
                    if (data.flag) {
                        $text.text($input.val());
                        var newNode = formatNode(data.node);
                        $.extend(node, newNode);
                    }

                    var properties = $(node.target).data('properties');
                    if (properties['lastName'] != $input.val()) {
                        var callbacks = $(node.target).data('callbacks');
                        if (callbacks['onRename'] && typeof callbacks['onRename'] === 'function') {
                            try {
                                callbacks['onRename'].call(null, $.extend({}, properties, {groupName: $input.val()}));
                                properties['lastName'] = $input.val();
                                properties['groupName'] = $input.val();

                                $input.remove();
                            } catch (ex) {
                                alert(ex.message);
                            }
                            ;
                        }
                    } else {
                        $input.remove();
                    }
                }
            });
        });

        $input.click(function () {
            $(this).select();
        }).trigger('click');
    };

    function removeFun(node) {
        leftOpt.onRemove.call(tree[0], {
            node: node,
            callback: function (dd) {
                var data = dd || {};
                if (data.flag) {
                    var properties = $(node.target).data('properties');
                    var callbacks = $(node.target).data('callbacks');

                    if (callbacks['onRemove'] && typeof callbacks['onRemove'] === 'function') {
                        try {
                            callbacks['onRemove'].call(null, properties);

                            var next = $(node.target).closest('li').next();
                            if (next.length != 0){
                                next.children('div').trigger('click');
                            }else{
                                $(node.target).closest('li').prev().children('div').trigger('click');
                            }

                            tree.tree('remove', node.target);
                            renderHandler();
                        } catch (ex) {
                            alert(ex.message);
                        }
                        ;
                    }
                }
            }
        });
    };

    function renderHandler() {
        $('.op-bar', $container).each(function () {
            renderDropDown($(this), panel);
        });
    };

    function renderPanel(id, panel) {
        var node = tree.tree('find', id);
        var html = '';
        var attr = node.attribute || {};

        if (attr.addable) {
            html += '<a rel="add" href="javascript:void(0);">新建亮灯分组</a>';
        }
        if (attr.editable) {
            html += '<a rel="edit" href="javascript:void(0);">重命名</a>';
        }
        if (attr.removeable) {
            html += '<a rel="remove" href="javascript:void(0);">删除</a>';
        }
        panel.html(html);

        $('a', panel).on('click', function () {
            switch ($(this).attr('rel')) {
                case 'add':
                    addFun(node);
                    break;
                case 'edit':
                    editFun(node);
                    break;
                case 'remove':
                    removeFun(node);
                    break;
            }
        });
    }

    function renderDropDown(handler, panel) {
        var hFlag = false, pFlag = false;
        handler.on({
            click: function () {
                return false;
            },
            mouseenter: function () {
                hFlag = true;
                setState();
            },
            mouseout: function () {
                setTimeout(function () {
                    hFlag = false;
                    setState();
                }, 0);
            }
        });

        panel.on('click', function () {
            pFlag = false;
            setState();
        }).hover(function () {
            pFlag = true;
            setState();
        }, function () {
            pFlag = false;
            setState();
        });

        function setState() {
            if (!handler.hasClass('visible') && hFlag && !pFlag) {
                var id = handler.attr('id');
                id = id.substring(0, id.length - opSuffix.length);

                renderPanel(id, panel);

                panel.css({
                    left: handler.offset().left + 1,
                    top: handler.offset().top + handler.outerHeight() - 2,
                    display: 'block'
                });
                handler.addClass('visible');
            } else if (handler.hasClass('visible') && !hFlag && !pFlag) {
                panel.hide();
                handler.removeClass('visible');
            }
        }
    };

    _me.init = function () {
        panel = $('.op-panel', $container);

        var firstLoad = false;

        opt.data = formatData(opt.data);
        opt.onLoadSuccess = function () {
            if (!firstLoad) {
                firstLoad = true;
                renderHandler();
                var root = $(this).tree('getRoot');
                if (root) {
                    $(this).tree('collapseAll').tree('expand', root.target);
                }
                $(this).tree('options').animate = true;
            }
        }

        tree = $('.left-tree', $container).tree(opt);
        return _me;
    };

    return _me;
};


var treeData = [
    {id: '1', pid: '-1', name: '宝贝操作权限', attribute: {addable: true, editable: true, removeable: true}},
    {id: '1-1', pid: '1', name: '宝贝操作权限', attribute: {addable: true, editable: true, removeable: true}},
    {id: '1-1-1', pid: '1-1', name: '宝贝操作权限', attribute: {editable: true, removeable: true}},
    {id: '1-1-2', pid: '1-1', name: '宝贝操作权限', attribute: {editable: true, removeable: true}},
    {id: '1-1-3', pid: '1-1', name: '宝贝操作权限', attribute: {editable: true, removeable: true}},
    {id: '1-1-4', pid: '1-1', name: '宝贝操作权限', attribute: {editable: true, removeable: true}},
    {id: '1-1-5', pid: '1-1', name: '宝贝操作权限', attribute: {editable: true, removeable: true}},
    {id: '1-1-6', pid: '1-1', name: '宝贝操作权限', attribute: {editable: true, removeable: true}},
    {id: '1-2', pid: '1', name: '宝贝管理', attribute: {addable: true, editable: true, removeable: true}},
    {id: '1-2-1', pid: '1-2', name: '宝贝管理', attribute: {addable: true, editable: true, removeable: true}},
    {id: '1-2-1-1', pid: '1-2-1', name: '宝贝管理X', attribute: {editable: true, removeable: true}},
    {id: '1-2-1-2', pid: '1-2-1', name: '宝贝管理Y', attribute: {editable: true, removeable: true}}
];
