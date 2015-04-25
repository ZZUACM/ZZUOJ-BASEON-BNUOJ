<?php
include_once("header.php");
include_once("functions/sidebars.php");
?>
        <div class="span12">
          <!-- insert the page content here -->
          <p>上次更新日期：<?= date ("Y-m-d H:i:s.", filemtime(__FILE__))?></p>
          <div class="">
            <ul class="nav nav-tabs" id="myTabs">
              <li><a href="#teaminfo">队伍简介</a></li>
              <li><a href="#memberinfo">队员现况</a></li>
              <li><a href="#honorinfo">ICPC成绩</a></li>
              <li><a id="tplan" href="#trainingplan">暑期训练计划</a></li>
            </ul>
            <div class="tab-content">
              <div class="tab-pane" id="memberinfo">
                <table id="membertable" class="table table-hover table-striped">
                  <thead>
                    <tr>
                      <th class="span2">姓名</th>
                      <th class="span2">年级</th>
                      <th class="span2">学院</th>
                      <th class="span6">经历与现况</th>
                    </tr>
                  </thead>
                  <tbody>
  <tr><td>卢凯鹏</td><td>2012级本</td><td>电气</td><td></td></tr>
                  </tbody>
                </table>
                <p style="align:right;text-align:right;padding:0;">
                  注：上表包括所有代表北京师范大学参加过现场赛的队员。
                </p>
              </div>
              <div class="tab-pane" id="teaminfo">
  <h3>ACM-ICPC 简介</h3>
  <p>
  ACM-ICPC 是由 ACM （美国计算机协会）主办的面向大学生的国际程序设计竞赛（International Collegiate Programming Contest），至今已举办了 36 届，被称为大学生的计算机奥林匹克竞赛。比赛由 3 名选手组成一队，在 5 小时内共用 1 台计算机编程比赛，特别强调团队配合和动手解决实际问题的能力，因此不仅被广泛认为是教学成果的体现，而且深受广大大学生的喜爱。目前，全国各知名高校都越来越重视本项赛事，大学生的参赛热情空前高涨。中国大陆地区的竞赛竞争最为激烈，水平居各大洲之首。
  </p>
  <h3>ACM-ICPC in ZZU</h3>
  <p>
  郑州大学ACM校队队员是来自信息工程学院、电气工程学院、数学统计学院、水利与环境学院等不同学科的学生，在比赛中充分发挥学科知识互补的优势，每年通过郑州大学ACM玲珑杯、郑州大学程序设计大赛等比赛选拔队员。
  </p>
  <p>
  长期以来，学校、教务处和信息学院给予 ACM 校队大力支持，提供了良好的训练条件，使得 ACM 校队的水平稳步上升。我校 ACM 校队最初由赵东明老师在 2008 年组建!</p> 
  </p>
              </div>
              <div class="tab-pane" id="honorinfo">
                <table id="honortable" class="table table-hover table-striped">
                  <thead>
                    <tr>
                      <th class="span1">年度</th>
                      <th class="span3">赛区（举办学校）</th>
                      <th class="span2">队名</th>
                      <th class="span3">队员</th>
                      <th class="span3">奖项（排名）</th>
                    </tr>
                  </thead>
                  <tbody>
  <tr><td>2013</td><td>长春赛区（吉林大学）</td><td>bufangqizhiliao</td><td>卢凯鹏 万方 张茂强</td><td>铜奖（铜奖区第2名）</td></tr>
                  </tbody>
                </table>
                <p style="align:right;text-align:right;padding:0;">
                  注：“*”号表示友情参赛，不计入ICPC排名。
                </p>
              </div>
              <div class="tab-pane" id="trainingplan">
                <p>以下是2013年暑期的训练计划，放在了Google Calendar上，加载较慢，看不见的同学请点<a href="https://www.google.com/calendar/embed?src=73ipa6fqajrvdfqnnhnlo2jac0%40group.calendar.google.com&ctz=Asia/Shanghai" target="_blank"><b>&gt;&gt;这里&lt;&lt;</b></a>查看。另外<b style="color:red">请务必仔细阅读页面下方的说明。</b></p>
                <p>积分看<a href="http://acm.bnu.edu.cn/v3/score2013.php" target="_blank"><b>&gt;&gt;这里&lt;&lt;</b></a></p>
                <div id="calendar"></div>
                <div>
                  一些说明：
                  <ol>
                    <li>参加暑期训练的每位同学都可以选择一至两周休息，具体时间队内协商。但注意积分赛的时间。</li>
                    <li>7.15-7.17将进行三场个人积分赛，题目由一二队准备。将根据积分赛的结果顺序进行组队，在参考各同学的组队意向的同时进行部分调整。</li>
                    <li>7.22开始进行进行组队积分赛，最终结合暑期组队赛积分（20%-40%）以及5场网络预赛的表现情况（60%-80%）决定参加区域赛的队伍。</li>
                  </ol>
                </div>
              </div>
            </div>
          </div>
        </div>

<script src='js/fullcalendar.min.js'></script>
<script src='js/gcal.js'></script>
<script>
$("#more").addClass("active");
$('#myTabs a:first').tab('show');
$('#myTabs a').click(function (e) {
  e.preventDefault();
  $(this).tab('show');
})
$("#tplan").click(function() {
    $('#calendar').html("").fullCalendar({
        events: 'http://www.google.com/calendar/feeds/73ipa6fqajrvdfqnnhnlo2jac0%40group.calendar.google.com/public/basic',
        className: 'gcal-event',
        aspectRatio: 2,
        firstDay: 1,
        year: 2013,
        month: 6
    });
});

if (self.document.location.hash.substring(1)=="trainingplan") $("#tplan").click();

$('#membertable').dataTable( {
    "bProcessing": true,
    "sDom": '<"row-fluid"pf>rt<"row-fluid"<"span8"i><"span4"l>>',
    "sPaginationType": "full_numbers" ,
    "aaSorting": [ [1, 'asc'] ],
    "iDisplayLength": 100,
    "iDisplayStart": 0
});
$('#honortable').dataTable( {
    "bProcessing": true,
    "sDom": '<"row-fluid"pf>rt<"row-fluid"<"span8"i><"span4"l>>',
    "sPaginationType": "full_numbers" ,
    "aaSorting": [ [0,'desc'] ],
    "iDisplayLength": 100,
    "iDisplayStart": 0
});
</script>

<?php
include_once("footer.php");
?>
