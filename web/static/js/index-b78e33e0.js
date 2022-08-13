import{z as e,b as t,o as a,c as n,R as o,r,S as l,a as i,Q as s,O as u,L as d,I as c,a1 as p,u as m,ae as h}from"./@vue-34da3461.js";import{E as f,a as v,b as g,c as y,d as b,e as w,f as _,g as x,h as V,i as k}from"./element-plus-479b58ed.js";import{u as C,i as S,a as O,b as I,c as M,d as A,e as j,f as T,g as W}from"./echarts-f18dd0f5.js";import{f as L,g as U}from"./@vueuse-2113b18a.js";import"./@element-plus-ce79297b.js";import"./@ctrl-eb0b847c.js";import"./lodash-es-e2bbebd4.js";import"./async-validator-efc2d198.js";import"./@popperjs-892fd7f5.js";import"./tslib-ed971494.js";import"./zrender-5be935e6.js";!function(){const e=document.createElement("link").relList;if(!(e&&e.supports&&e.supports("modulepreload"))){for(const e of document.querySelectorAll('link[rel="modulepreload"]'))t(e);new MutationObserver((e=>{for(const a of e)if("childList"===a.type)for(const e of a.addedNodes)"LINK"===e.tagName&&"modulepreload"===e.rel&&t(e)})).observe(document,{childList:!0,subtree:!0})}function t(e){if(e.ep)return;e.ep=!0;const t=function(e){const t={};return e.integrity&&(t.integrity=e.integrity),e.referrerpolicy&&(t.referrerPolicy=e.referrerpolicy),"use-credentials"===e.crossorigin?t.credentials="include":"anonymous"===e.crossorigin?t.credentials="omit":t.credentials="same-origin",t}(e);fetch(e.href,t)}}();const P=["id"];var D,G,z;C([S,O,I,M,A,j,T]);let N=3.3,B=1.65,E=50,F=1;var R=new Array,q=new Array;function J(e){if(1==e)for(var t=0,a=0;a<256;a++)t=Math.sin(2*Math.PI/256*a)*(N/2)+B,R[a]=255*t/3.3;else if(2==e){var n=E/100*256;for(a=0;a<256;a++)R[a]=a<n?255*(N/2+B)/3.3:255*(-N/2+B)/3.3}else if(3==e)for(a=-128;a<128;a++)R[a+128]=a*(N/3.3)+255*B/3.3;for(a=0;a<256;a++)R[a]>255&&(R[a]=255),R[a]<0&&(R[a]=0),q[a]=3.3*R[a]/255}function Z(){let e=[];for(let t=0;t<=256;t++)e.push([t,q[t]]);return e}function $(){B=ce.biasVoltage.value,E=ce.duty.value,N=ce.uMaxValue.value,F=parseInt(ce.waveType.value),J(F),G.setOption({series:[{data:Z()}]})}const H=e({...{name:"WaveGenChart",option:D={backgroundColor:"",animation:!1,grid:{top:40,left:50,right:40,bottom:50},tooltip:{show:!0,trigger:"axis",axisPointer:{type:"cross"}},toolbox:{show:!0,feature:{dataZoom:{yAxisIndex:"none"},dataView:{readOnly:!1},magicType:{type:["line","line"]},restore:{},saveAsImage:{}}},xAxis:{name:"波形索引",min:0,max:256,minorTick:{show:!0},minorSplitLine:{show:!0}},yAxis:{name:"输出电压/V",min:-.2,max:3.5,minorTick:{show:!0},minorSplitLine:{show:!0}},dataZoom:[{show:!0,type:"inside",filterMode:"none",xAxisIndex:[0],startValue:0,endValue:300,moveOnMouseWheel:"alt"},{show:!0,type:"inside",filterMode:"none",yAxisIndex:[0],startValue:-1,endValue:5}],series:[{type:"line",showSymbol:!1,clip:!0,data:Z()}]},uMaxValue:N,offSetValue:B,duty:E,wave:F,refreshData:$,darkMode:function(e){G.dispose(),(G=1==e?W(z,"dark"):W(z)).setOption(D),$()}},props:{width:{type:String,default:"550"},height:{type:String,default:"350"},isFirst:{type:Boolean,default:!1},container:{type:String,default:"WaveGenChart"}},setup(e){const r=e;return t((()=>{J(1),z=document.getElementById(r.container),G=W(z),D&&G.setOption(D)})),(t,r)=>(a(),n("div",{style:o(`width:${e.width}px;height:${e.height}px;`),id:e.container,class:"WaveGenChart"},null,12,P))}}),K={websocket:null,connectURL:"ws://192.168.8.1:80",socket_open:!1,hearbeat_timer:null,hearbeat_interval:45e3,is_reonnect:!0,reconnect_count:3,reconnect_current:1,reconnect_number:0,reconnect_timer:null,reconnect_interval:5e3,init:e=>{if(!("WebSocket"in window))return f.warning("浏览器不支持WebSocket"),null;K.websocket=new WebSocket(K.connectURL),K.websocket.onmessage=t=>{e&&e(t)},K.websocket.onclose=e=>{clearInterval(K.hearbeat_interval),K.socket_open=!1,K.is_reonnect&&(K.reconnect_timer=setTimeout((()=>{if(K.reconnect_current>K.reconnect_count)return clearTimeout(K.reconnect_timer),void(K.is_reonnect=!1);K.reconnect_current++,K.reconnect()}),K.reconnect_interval))},K.websocket.onopen=function(){K.socket_open=!0,K.is_reonnect=!0,f({type:"success",message:"连接成功！",duration:3e3}),this.send("ID1")},K.websocket.onerror=function(){f({type:"error",message:"连接发生错误,正在尝试重连",duration:0,center:!0,grouping:!0,showClose:!0})}},send:(e,t=null)=>{K.websocket.readyState===K.websocket.OPEN?(K.websocket.send(e),t&&t()):(clearInterval(K.hearbeat_timer),f({type:"error",message:"请先建立连接",duration:0,center:!0,grouping:!0,showClose:!0}),K.reconnect_number++)},receive:e=>{let t=e.data;return t=JSON.parse(t),t},heartbeat:()=>{K.hearbeat_timer&&clearInterval(K.hearbeat_timer),K.hearbeat_timer=setInterval((()=>{var e={encryption_type:"none",data:JSON.stringify({content:"ping"})};K.send(e)}),K.hearbeat_interval)},close:()=>{clearInterval(K.hearbeat_interval),K.is_reonnect=!1,K.websocket.close(),f({type:"success",message:"已断开连接",duration:0,center:!0,grouping:!0,showClose:!0})},reconnect:()=>{K.websocket&&!K.is_reonnect&&K.close(),K.init(null)}},Q={class:"waveGen"},X=p("立即设置"),Y=p("获取参数"),ee=r("large"),te=r("right"),ae=r("1"),ne=r(100),oe=r(50),re=r(3.3),le=r(1.65),ie=l({waveType:"1",freq:r(100),duty:r(50),uMaxValue:r(3.3),biasVoltage:r(1.65)}),se=e=>{K.socket_open?K.send(e):f({type:"error",message:"请先建立连接",duration:0,center:!0,grouping:!0,showClose:!0})},ue=()=>{H.refreshData()},de=()=>{se("GP")},ce=e({...{name:"WaveGen",waveType:ae,freq:ne,duty:oe,uMaxValue:re,biasVoltage:le,changeParam:e=>{re.value=e.U,le.value=e.B,oe.value=e.D,ne.value=e.F,ae.value=String(e.W),H.refreshData()},props1:{msg:String}},props:{msg:null},setup:e=>(t,o)=>{const r=v,l=g,p=w,m=y,h=b,f=_;return a(),n(c,null,[i("h2",null,s(e.msg),1),i("div",Q,[u(f,{model:ie,"label-width":"auto",size:ee.value,"label-position":te.value},{default:d((()=>[u(p,{label:"波形选择"},{default:d((()=>[u(l,{modelValue:ae.value,"onUpdate:modelValue":o[0]||(o[0]=e=>ae.value=e),onChange:o[1]||(o[1]=e=>{return t=ae.value,ie.waveType=t,void se("W"+t);var t}),placeholder:"请选择波形"},{default:d((()=>[u(r,{label:"正弦波",value:"1"}),u(r,{label:"方波",value:"2"}),u(r,{label:"锯齿波",value:"3"})])),_:1},8,["modelValue"])])),_:1}),u(p,{label:"频率",style:{width:"100%"}},{default:d((()=>[u(m,{modelValue:ne.value,"onUpdate:modelValue":o[2]||(o[2]=e=>ne.value=e),onChange:o[3]||(o[3]=e=>{return t=ne.value,ie.freq=t,void se("F"+t);var t}),min:1,max:1500,step:1,label:"频率"},null,8,["modelValue"])])),_:1}),u(p,{label:"占空比"},{default:d((()=>[u(m,{modelValue:oe.value,"onUpdate:modelValue":o[4]||(o[4]=e=>oe.value=e),onChange:o[5]||(o[5]=e=>{return t=oe.value,ie.duty=t,void se("D"+t);var t}),min:1,max:99,step:1,label:"占空比"},null,8,["modelValue"])])),_:1}),u(p,{label:"峰峰值"},{default:d((()=>[u(m,{modelValue:re.value,"onUpdate:modelValue":o[6]||(o[6]=e=>re.value=e),onChange:o[7]||(o[7]=e=>{return t=re.value,ie.uMaxValue=t,void se("U"+t);var t}),min:0,max:3.3,precision:2,step:.2,label:"峰峰值"},null,8,["modelValue","max","step"])])),_:1}),u(p,{label:"偏置电压"},{default:d((()=>[u(m,{modelValue:le.value,"onUpdate:modelValue":o[8]||(o[8]=e=>le.value=e),onChange:o[9]||(o[9]=e=>{return t=le.value,void se("B"+t);var t}),min:0,max:3.3,precision:2,step:.2,label:"频率"},null,8,["modelValue","max","step"])])),_:1}),u(p,null,{default:d((()=>[u(h,{type:"primary",onClick:ue,style:{width:"45%","margin-right":"5%"}},{default:d((()=>[X])),_:1}),u(h,{onClick:de,style:{width:"45%","margin-left":"5%"}},{default:d((()=>[Y])),_:1})])),_:1})])),_:1},8,["model","size","label-position"])])],64)}}),pe=["id"],me={class:"OSCForm"},he=p("运行"),fe=p("停止");var ve,ge,ye;let be=!0;var we=new Array;const _e=r(8e3);function xe(){be=!0,Ce("CT")}function Ve(){be=!1,Ce("CF")}function ke(){let e=[];if(we.length<10)for(var t=0;t<256;t++)we[t]=0,e.push([t,we[t]]);else for(t=0;t<256;t++)e.push([t,we[t]]);return e}ve={backgroundColor:"",animation:!1,grid:{top:40,left:50,right:40,bottom:50},tooltip:{show:!0,trigger:"axis",axisPointer:{type:"cross"}},toolbox:{show:!0,feature:{dataZoom:{yAxisIndex:"none"},dataView:{readOnly:!1},magicType:{type:["line","line"]},restore:{},saveAsImage:{}}},xAxis:{name:"Div",min:0,max:256,minorTick:{show:!0},minorSplitLine:{show:!0}},yAxis:{name:"电压/V",min:-.2,max:3.5,minorTick:{show:!0},minorSplitLine:{show:!0}},dataZoom:[{show:!0,type:"inside",filterMode:"none",xAxisIndex:[0],startValue:0,endValue:40},{show:!0,type:"inside",filterMode:"none",yAxisIndex:[0],startValue:-1,endValue:5}],series:[{type:"line",showSymbol:!1,clip:!0,data:ke()}]};const Ce=e=>{K.socket_open?K.send(e):f({type:"error",message:"请先建立连接",duration:0,center:!0,grouping:!0,showClose:!0})};const Se=e({...{name:"OSChart",darkMode:function(e){ge.dispose(),(ge=1==e?W(ye,"dark"):W(ye)).setOption(ve),ge.setOption({series:[{data:ke()}]})},refreshData:function(e){be&&(we=[],we=e.a,ge.setOption({series:[{data:ke()}]}))},changeParam:e=>{_e.value=e.R},props2:{msg:String,option:Object}},props:{width:{type:String,default:"550"},height:{type:String,default:"350"},isFirst:{type:Boolean,default:!1},container:{type:String,default:"OSCChart"}},setup(e){const r=e;return C([S,O,I,M,A,j,T]),t((()=>{ye=document.getElementById(r.container),ge=W(ye),ve&&ge.setOption(ve)})),(t,r)=>{const l=b,s=w,p=y;return a(),n(c,null,[i("div",{style:o(`width:${e.width}px;height:${e.height}px;`),id:e.container,class:"OSChart"},null,12,pe),i("div",me,[u(s,{size:"large"},{default:d((()=>[u(l,{type:"primary",onClick:xe,style:{width:"45%","margin-right":"5%"}},{default:d((()=>[he])),_:1}),u(l,{onClick:Ve,style:{width:"45%","margin-left":"5%"}},{default:d((()=>[fe])),_:1})])),_:1}),u(s,{size:"large",label:"采样频率/Hz"},{default:d((()=>[u(p,{modelValue:_e.value,"onUpdate:modelValue":r[0]||(r[0]=e=>_e.value=e),onChange:r[1]||(r[1]=e=>{return t=_e.value,void Ce("R"+t);var t}),min:1e3,max:128e3,step:1e3,"step-strictly":"",label:"采样频率/Hz"},null,8,["modelValue"])])),_:1})])],64)}}}),Oe={class:"container",style:{"justify-content":"normal"}},Ie={class:"LeftMain"},Me=i("h2",null,"ESP32示波器面板",-1);h(e({...{name:"App",components:{WaveGen:ce,WaveGenChart:H,OSChart:Se}},setup(e){const o=L(),l=U(o),s=r(o.value),c=r(!1);function p(){H.darkMode(o.value),Se.darkMode(o.value)}function h(e){if("{"==e.data[0]&&"a"==e.data[2]){var t=JSON.parse(e.data);Se.refreshData(t)}else if("{"==e.data[0]&&"p"==e.data[2]){t=JSON.parse(e.data);ce.changeParam(t.param),Se.changeParam(t.param)}}return t((()=>{p(),s.value=o.value})),(e,t)=>{const o=x,r=V,f=w,v=k;return a(),n("div",Oe,[u(v,null,{default:d((()=>[u(o,{span:12},{default:d((()=>[i("div",Ie,[u(ce,{msg:"波形发生器控制面板"}),u(H,{width:"550",height:"350",container:"WaveGenChart"})])])),_:1}),u(o,{span:12},{default:d((()=>[u(v,{justify:"end",style:{height:"32px"}},{default:d((()=>[u(f,{label:"连接设备",style:{"margin-right":"20px"}},{default:d((()=>[u(r,{modelValue:c.value,"onUpdate:modelValue":t[0]||(t[0]=e=>c.value=e),onChange:t[1]||(t[1]=e=>{1==c.value?K.init(h):K.close()}),"inline-prompt":"","active-text":"开","inactive-text":"关"},null,8,["modelValue"])])),_:1}),u(f,{label:"深色模式",style:{"margin-right":"20px"}},{default:d((()=>[u(r,{modelValue:s.value,"onUpdate:modelValue":t[2]||(t[2]=e=>s.value=e),onChange:t[3]||(t[3]=e=>(m(l)(),p())),"inline-prompt":"","active-text":"开","inactive-text":"关"},null,8,["modelValue"])])),_:1})])),_:1}),u(v,null,{default:d((()=>[u(o,{span:24},{default:d((()=>[Me,u(Se,{width:"550",height:"350",container:"OSChart"})])),_:1})])),_:1})])),_:1})])),_:1})])}}})).mount("#app");