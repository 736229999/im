<%--
  Created by IntelliJ IDEA.
  User: wangzhen
  Date: 2015/4/15
  Time: 15:30
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <title>快捷短语</title>
    <link rel="stylesheet" type="text/css" href="../../js/jeasyui/themes/default/easyui.css"/>
    <link rel="stylesheet" type="text/css" href="../../js/jeasyui/themes/icon.css"/>
    <link href="../../css/content.css" rel="stylesheet" type="text/css"/>
    <link href="../../css/base.css" rel="stylesheet" type="text/css"/>
    <link rel="stylesheet" href="../../css/font-awesome.min.css" type="text/css"/>
    <link rel="stylesheet" href="../../css/relative-settings/phrase.css" type="text/css"/>
</head>
<body>
<div id="main_warp">
    <!--查询条件-->
    <div class="cla_nav"><span class="on">查询条件</span></div>
    <div></div>
    <!--条件搜索-->
    <div class="searchbar">
        <ul>
            <li class="pull-left">
                <span>快捷编码：</span>
                <input type="text" id="quickCode" class="easyui-validatebox seller_nick1" value=""
                       auto-complete="off" data-name="" autocomplete="off">
            <li>
                <span>快捷短语关键字：</span>
                <input type="text" id="phrasePart" class="easyui-validatebox seller_nick1" value=""
                       auto-complete="off" data-name="" autocomplete="off">
            </li>
            </li>
            <li>
                <span>分组名称：</span>
                <input type="text" id="groupSearch" class="easyui-combobox seller_nick1 groupCombo" value=""
                       style="height: 28px"
                       data-name="分组名称" autocomplete="off">
            </li>
            <li>
                <input id="search" type="button" class="search_btn" value="查询"/>
                <input id="resetCondition" type="button" class="rewrite_btn" value="重置"/>
            </li>
        </ul>
    </div>
    <!--导出excel表格按钮-->
    <div>
        <input type="button" id="addPhrase" class="operation_btn_bg add_btn_2" value="添加">
        <input type="button" id="deletePhrases" class="operation_btn_bg del_btn_2" value="删除">
        <input type="button" id="excelImport" class="operation_btn_bg add_btn_2" value="导入">
        <input type="button" id="excelExport" class="export_btn_bg export_btn_4" value="导出">
        <%--<span class="btn add_btn_2" id="addGroup">--%>
            <%--<em class="fa fa-plus"></em>&nbsp;新建分组--%>
        <%--</span>--%>
        <span class="btn add_btn_2" id="editGroup">
            <em class="fa fa-pencil"></em>&nbsp;修改分组名称
        </span>
    </div>
    <!--结果-->
    <div id="content">
        <div class="padd_b_10">
            <div id="phraseTable" style="width: 100%;"></div>
        </div>
    </div>

    <!-- 快捷短语编辑窗口 -->
    <div id="phraseEditWindow" class="easyui-window"
         data-options="collapsible : false, minimizable : false, maximizable : false, closed: true,
             closable : true, modal : true, title: '添加/修改快捷短语'" style="width: 550px ; display: none">
        <div class="wrapper">
            <div class="phrase-field">
                <span class="phrase-label"></span>快捷编码：</span>
                <input name="quickCode" placeholder=""/>
                <span class="phrase-error">X 不符合填写要求</span>

                <div>
                    <span class="phrase-notice">支持数字、字母、中文，不超过4个字</span>
                </div>
            </div>
            <div class="phrase-field">
                <span class="phrase-label"><span class="required">*</span>快捷短语：</span>

                <div class="toolbar">
                    <select class="font-family-select" act="font-family">
                        <option value="SimSun">宋体</option>
                        <option value="SimHei">黑体</option>
                        <option value="Microsoft YaHei" selected>微软雅黑</option>
                    </select>
                    <select class="font-size-select" act="font-size">
                        <option value="9">9</option>
                        <option value="10">10</option>
                        <option value="11">11</option>
                        <option value="12" selected>12</option>
                        <option value="13">13</option>
                        <option value="14">14</option>
                        <option value="15">15</option>
                        <option value="16">16</option>
                        <option value="17">17</option>
                        <option value="18">18</option>
                        <option value="19">19</option>
                        <option value="20">20</option>
                        <option value="21">21</option>
                        <option value="22">22</option>
                    </select>
                    <a href="javascript:void(0);" class="b" act="b"></a>
                    <a href="javascript:void(0);" class="i" act="i"></a>
                    <a href="javascript:void(0);" class="u" act="u"></a>
                    <a href="javascript:void(0);" class="color" act="color"></a>
                    <a href="javascript:void(0);" class="face" act="face"></a>
                    <!-- 表情弹出窗口 -->
                    <div id="J_face_popup" class="face_popup" style="display: none;">
                        <table class="heads">
                            <tr>
                                <td><a href="javascript:void(0);" title="大笑"><img/></a>
                                </td>
                                <td><a href="javascript:void(0);" title="害羞"><img/></a>
                                </td>
                                <td><a href="javascript:void(0);" title="欢喜"><img/></a>
                                </td>
                                <td><a href="javascript:void(0);" title="开心"><img/></a>
                                </td>
                                <td><a href="javascript:void(0);" title="流汗"><img/></a>
                                </td>
                                <td><a href="javascript:void(0);" title="流泪"><img/></a>
                                </td>
                                <td><a href="javascript:void(0);" title="牛B"><img/></a></td>
                                <td><a href="javascript:void(0);" title="牛B"><img/></a></td>
                                <td><a href="javascript:void(0);" title="牛B"><img/></a></td>
                                <td><a href="javascript:void(0);" title="牛B"><img/></a></td>
                            </tr>
                            <tr>
                                <td><a href="javascript:void(0);" title="大笑"><img/></a>
                                </td>
                                <td><a href="javascript:void(0);" title="害羞"><img/></a>
                                </td>
                                <td><a href="javascript:void(0);" title="欢喜"><img/></a>
                                </td>
                                <td><a href="javascript:void(0);" title="开心"><img/></a>
                                </td>
                                <td><a href="javascript:void(0);" title="流汗"><img/></a>
                                </td>
                                <td><a href="javascript:void(0);" title="流泪"><img/></a>
                                </td>
                                <td><a href="javascript:void(0);" title="牛B"><img/></a></td>
                                <td><a href="javascript:void(0);" title="牛B"><img/></a></td>
                                <td><a href="javascript:void(0);" title="牛B"><img/></a></td>
                                <td><a href="javascript:void(0);" title="牛B"><img/></a></td>
                            </tr>
                            <tr>
                                <td><a href="javascript:void(0);" title="大笑"><img/></a>
                                </td>
                                <td><a href="javascript:void(0);" title="害羞"><img/></a>
                                </td>
                                <td><a href="javascript:void(0);" title="欢喜"><img/></a>
                                </td>
                                <td><a href="javascript:void(0);" title="开心"><img/></a>
                                </td>
                                <td><a href="javascript:void(0);" title="流汗"><img/></a>
                                </td>
                                <td><a href="javascript:void(0);" title="流泪"><img/></a>
                                </td>
                                <td><a href="javascript:void(0);" title="牛B"><img/></a></td>
                                <td><a href="javascript:void(0);" title="牛B"><img/></a></td>
                                <td><a href="javascript:void(0);" title="牛B"><img/></a></td>
                                <td><a href="javascript:void(0);" title="牛B"><img/></a></td>
                            </tr>
                            <tr>
                                <td><a href="javascript:void(0);" title="大笑"><img/></a>
                                </td>
                                <td><a href="javascript:void(0);" title="害羞"><img/></a>
                                </td>
                                <td><a href="javascript:void(0);" title="欢喜"><img/></a>
                                </td>
                                <td><a href="javascript:void(0);" title="开心"><img/></a>
                                </td>
                                <td><a href="javascript:void(0);" title="流汗"><img/></a>
                                </td>
                                <td><a href="javascript:void(0);" title="流泪"><img/></a>
                                </td>
                                <td><a href="javascript:void(0);" title="牛B"><img/></a></td>
                                <td><a href="javascript:void(0);" title="牛B"><img/></a></td>
                                <td><a href="javascript:void(0);" title="牛B"><img/></a></td>
                                <td><a href="javascript:void(0);" title="牛B"><img/></a></td>
                            </tr>
                            <tr>
                                <td><a href="javascript:void(0);" title="大笑"><img/></a>
                                </td>
                                <td><a href="javascript:void(0);" title="害羞"><img/></a>
                                </td>
                                <td><a href="javascript:void(0);" title="欢喜"><img/></a>
                                </td>
                                <td><a href="javascript:void(0);" title="开心"><img/></a>
                                </td>
                                <td><a href="javascript:void(0);" title="流汗"><img/></a>
                                </td>
                                <td><a href="javascript:void(0);" title="流泪"><img/></a>
                                </td>
                                <td><a href="javascript:void(0);" title="牛B"><img/></a></td>
                                <td><a href="javascript:void(0);" title="牛B"><img/></a></td>
                                <td><a href="javascript:void(0);" title="牛B"><img/></a></td>
                                <td><a href="javascript:void(0);" title="牛B"><img/></a></td>
                            </tr>
                            <tr>
                                <td><a href="javascript:void(0);" title="大笑"><img/></a>
                                </td>
                                <td><a href="javascript:void(0);" title="害羞"><img/></a>
                                </td>
                                <td><a href="javascript:void(0);" title="欢喜"><img/></a>
                                </td>
                                <td><a href="javascript:void(0);" title="开心"><img/></a>
                                </td>
                                <td><a href="javascript:void(0);" title="流汗"><img/></a>
                                </td>
                            </tr>
                        </table>
                    </div>
                </div>
                <div name="phraseSummary" class="summary" rows="5" placeholder="请填写解释" contenteditable="true"
                     style="font-family: Microsoft Yahei; font-size: 12px; font-style:normal; text-decoration: none; font-weight: normal;">
                </div>
                <div>
                    <span class="phrase-error">X 请输入快捷短语</span>
                </div>
            </div>
            <div class="phrase-field">
                <div>
                    <span class="phrase-label"><span class="required">*</span>分组:</span>
                    <input type="text" id="phraseGroup" class="easyui-combobox seller_nick1 groupCombo" value=""
                           style="height: 28px" data-name="分组名称" autocomplete="off">
                    <span class="phrase-error">X 请选择分组</span>
                </div>
            </div>
            <div style="text-align:center;padding:5px">
                <input id="editPhraseBtn" class="operation_btn_bg con_btn_2" value="保存"/>
                <input id="returnBtn" class="operation_btn_bg ret_btn_2" value="返回"/>
            </div>
        </div>
    </div>

    <!-- 分组编辑窗口-->
    <div id="quickPhraseGroupEditWindow" class="easyui-window"
         data-options="collapsible : false, minimizable : false, maximizable : false, closed: true,
             closable : true, modal : true, title: '修改分组名称'" style="width: 600px; display: none">
        <div id="groupToolbar" style="width: 100%;padding: 15px 5px; height: 58px;box-sizing: border-box">
            <span class="phrase-label"
                  style="height: 28px;line-height: 28px;font-size: 14px;float: left;margin-right: 5px">分组名称:</span>
            <input class="seller_nick1" type="text" id="groupNamePart" style="float: left;margin-right: 15px"/>
            <input id="searchGroup" type="button" class="search_btn" value="查询"/>
            <input id="resetConditionGroup" type="button" class="rewrite_btn" value="重置"/>
        </div>
        <div id="contentGroup">
            <div class="padd_b_10" style="width: 100%;padding: 0px 5px 15px; box-sizing: border-box">
                <div id="groupTable" style="width: 100%">
                </div>
            </div>
        </div>
    </div>

    <%--<div id="addGroupWindow" class="easyui-window"--%>
         <%--data-options="collapsible : false, minimizable : false, maximizable : false, closed: true,--%>
             <%--closable : true, modal : true, title: '新增分组'" style="width: 450px; display: none">--%>
        <%--<div class="wrapper" style="padding: 15px 5px;">--%>
            <%--<div class="group-field">--%>
                <%--<div>--%>
                    <%--<span class="group-label"><span class="required">*</span>新增分组名称:</span>--%>
                    <%--<input type="text" id="newGroupName" value="" autocomplete="off">--%>
                    <%--<span class="group-error">X 10个字以内</span>--%>
                <%--</div>--%>
            <%--</div>--%>
        <%--</div>--%>
        <%--<div style="text-align:center;padding:5px">--%>
            <%--<input id="addGroupSaveBtn" class="operation_btn_bg con_btn_2" value="保存"/>--%>
            <%--<input id="addGroupReturnBtn" class="operation_btn_bg ret_btn_2" value="返回"/>--%>
        <%--</div>--%>
    <%--</div>--%>

    <div id="confirmDeleteWindow" class="easyui-window"
         data-options="collapsible : false, minimizable : false, maximizable : false, closed: true,
             closable : true, modal : true, title: '确认是否删除'" style="width: 300px;display: none">
        <p class="content confirm-delete-panel">确认删除这条数据吗？</p>

        <div style="text-align:center;padding:5px 5px 10px;">
            <input id="confirmDelete" class="confirm-btn" value="确认"/>
            <input id="cancelDelete" class="confirm-btn cancel" value="取消"/>
        </div>
    </div>
</div>

<script type="text/javascript" src="../../js/jquery.min.js"></script>
<script type="text/javascript" src="../../js/jeasyui/jquery.easyui.min.js"></script>
<script type="text/javascript" src="../../js/jeasyui/validateExtend.js"></script>
<script type="text/javascript" src="../../js/jeasyui/locale/easyui-lang-zh_CN.js"></script>
<script type="text/javascript" src="../../js/jeasyui/validateExtend.js"></script>
<script type="text/javascript" src="../../js/myCalendar/WdatePicker.js"></script>
<script type="text/javascript" src="../../js/util/generalUtil.js"></script>
<script type="text/javascript" src="../../js/util/easyuiUtil.js"></script>
<script type="text/javascript" src="../../js/plugin/uploader/jquery.uploader.js"></script>
<script type="text/javascript" src="../../js/plugin/echarts/echarts-all.js"></script>
<script type="text/javascript" src="../../js/plugin/colorpicker/jquery-colorpicker.js"></script>
<script type="text/javascript" src="../../js/relative-settings/phrase.js"></script>
</body>
</html>
