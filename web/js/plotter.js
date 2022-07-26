let data1 = [];
function generateData(value1) {
  value = angle_value/10;
  return [value1, value];
}
function getData() {
  let nowTime = new Date();
}
option1 = {
  animation: false,
  tooltip: {
    trigger: 'axis'
  },
  xAxis: {
    name: 'ms',
    type: 'value',
    min: 0,
    max: 40,
    splitLine: {
      show: true
    },
    minorTick: {
      show: true
    },
    minorSplitLine: {
      show: true
    }
  },
  yAxis: {
    name: '目标值/V',
    type: 'value',
    min: -1,
    max: 8,
    minorTick: {
      show: true
    },
    minorSplitLine: {
      show: true
    }
  },
  dataZoom: [{
    show: true,
    type: 'inside',
    filterMode: 'none',
    xAxisIndex: [0],
    startValue: 0,
    endValue: 40
  }, {
    show: true,
    type: 'inside',
    filterMode: 'none',
    yAxisIndex: [0],
    startValue: 0,
    endValue: 10,
    minSpan: 0.5
  }],
  series: [
    {
      type: 'line',
      showSymbol: false, //是否显示每个点
      clip: true,
      data: data1
    }
  ]
};
//创建plotter对象
var Plotter = {
  timer: '',
  myChart: '',
  chartDom: '',
  init: function (domID, option) {
    this.domID = domID; this.option = option;
    chartDom = document.getElementById(this.domID);
    myChart = echarts.init(chartDom);
    myChart.setOption(this.option);

  },
  start: function () { //启动数据更新
    timer = setInterval(function () {
      if (data1.length >= 100) {
        data1.shift();
      }
      data1.push(generateData());
      myChart.setOption({
        series: [
          {
            data: data1
          }
        ]
      });
    }, 1000);
  },
  stop: function () {  //关闭数据更新
    clearInterval(timer);
  },
  refresh: function () {
    if (data1.length >= 40) {
      data1.shift();
      for (var i = 0; i < data1.length; i++) {
        data1[i][0] -= 1;
      }
      data1.push([data1.length, angle_value/10]);
    }
    else {
      data1.push(generateData(data1.length));
    }

    myChart.setOption({
      series: [
        {
          data: data1
        }
      ]
    });
  }

}
Plotter.init('echartsMain', option1);
//Plotter.start();





