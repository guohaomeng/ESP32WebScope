const t="object"==typeof global&&global&&global.Object===Object&&global;var e="object"==typeof self&&self&&self.Object===Object&&self;const r=t||e||Function("return this")();const n=r.Symbol;var o=Object.prototype,c=o.hasOwnProperty,a=o.toString,u=n?n.toStringTag:void 0;var i=Object.prototype.toString;var f=n?n.toStringTag:void 0;function s(t){return null==t?void 0===t?"[object Undefined]":"[object Null]":f&&f in Object(t)?function(t){var e=c.call(t,u),r=t[u];try{t[u]=void 0;var n=!0}catch(i){}var o=a.call(t);return n&&(e?t[u]=r:delete t[u]),o}(t):function(t){return i.call(t)}(t)}function l(t){return null!=t&&"object"==typeof t}function p(t){return"symbol"==typeof t||l(t)&&"[object Symbol]"==s(t)}const b=Array.isArray;var v=n?n.prototype:void 0,y=v?v.toString:void 0;function h(t){if("string"==typeof t)return t;if(b(t))return function(t,e){for(var r=-1,n=null==t?0:t.length,o=Array(n);++r<n;)o[r]=e(t[r],r,t);return o}(t,h)+"";if(p(t))return y?y.call(t):"";var e=t+"";return"0"==e&&1/t==-Infinity?"-0":e}var j=/\s/;var d=/^\s+/;function _(t){return t?t.slice(0,function(t){for(var e=t.length;e--&&j.test(t.charAt(e)););return e}(t)+1).replace(d,""):t}function g(t){var e=typeof t;return null!=t&&("object"==e||"function"==e)}var w=/^[-+]0x[0-9a-f]+$/i,O=/^0b[01]+$/i,m=/^0o[0-7]+$/i,A=parseInt;function x(t){if("number"==typeof t)return t;if(p(t))return NaN;if(g(t)){var e="function"==typeof t.valueOf?t.valueOf():t;t=g(e)?e+"":e}if("string"!=typeof t)return 0===t?t:+t;t=_(t);var r=O.test(t);return r||m.test(t)?A(t.slice(2),r?2:8):w.test(t)?NaN:+t}function S(t){if(!g(t))return!1;var e=s(t);return"[object Function]"==e||"[object GeneratorFunction]"==e||"[object AsyncFunction]"==e||"[object Proxy]"==e}const z=r["__core-js_shared__"];var P,E=(P=/[^.]+$/.exec(z&&z.keys&&z.keys.IE_PROTO||""))?"Symbol(src)_1."+P:"";var T=Function.prototype.toString;function I(t){if(null!=t){try{return T.call(t)}catch(e){}try{return t+""}catch(e){}}return""}var F=/^\[object .+?Constructor\]$/,M=Function.prototype,U=Object.prototype,$=M.toString,k=U.hasOwnProperty,B=RegExp("^"+$.call(k).replace(/[\\^$.*+?()[\]{}|]/g,"\\$&").replace(/hasOwnProperty|(function).*?(?=\\\()| for .+?(?=\\\])/g,"$1.*?")+"$");function D(t){return!(!g(t)||(e=t,E&&E in e))&&(S(t)?B:F).test(I(t));var e}function N(t,e){var r=function(t,e){return null==t?void 0:t[e]}(t,e);return D(r)?r:void 0}const C=N(r,"WeakMap");var L=Object.create,V=function(){function t(){}return function(e){if(!g(e))return{};if(L)return L(e);t.prototype=e;var r=new t;return t.prototype=void 0,r}}();const W=V;var R=function(){try{var t=N(Object,"defineProperty");return t({},"",{}),t}catch(e){}}();const q=R;var G=/^(?:0|[1-9]\d*)$/;function H(t,e){var r=typeof t;return!!(e=null==e?9007199254740991:e)&&("number"==r||"symbol"!=r&&G.test(t))&&t>-1&&t%1==0&&t<e}function J(t,e,r){"__proto__"==e&&q?q(t,e,{configurable:!0,enumerable:!0,value:r,writable:!0}):t[e]=r}function K(t,e){return t===e||t!=t&&e!=e}var Q=Object.prototype.hasOwnProperty;function X(t,e,r){var n=t[e];Q.call(t,e)&&K(n,r)&&(void 0!==r||e in t)||J(t,e,r)}function Y(t,e,r,n){var o=!r;r||(r={});for(var c=-1,a=e.length;++c<a;){var u=e[c],i=n?n(r[u],t[u],u,r,t):void 0;void 0===i&&(i=t[u]),o?J(r,u,i):X(r,u,i)}return r}function Z(t){return"number"==typeof t&&t>-1&&t%1==0&&t<=9007199254740991}function tt(t){return null!=t&&Z(t.length)&&!S(t)}var et=Object.prototype;function rt(t){var e=t&&t.constructor;return t===("function"==typeof e&&e.prototype||et)}function nt(t){return l(t)&&"[object Arguments]"==s(t)}var ot=Object.prototype,ct=ot.hasOwnProperty,at=ot.propertyIsEnumerable;const ut=nt(function(){return arguments}())?nt:function(t){return l(t)&&ct.call(t,"callee")&&!at.call(t,"callee")};var it="object"==typeof exports&&exports&&!exports.nodeType&&exports,ft=it&&"object"==typeof module&&module&&!module.nodeType&&module,st=ft&&ft.exports===it?r.Buffer:void 0;const lt=(st?st.isBuffer:void 0)||function(){return!1};var pt={};function bt(t){return function(e){return t(e)}}pt["[object Float32Array]"]=pt["[object Float64Array]"]=pt["[object Int8Array]"]=pt["[object Int16Array]"]=pt["[object Int32Array]"]=pt["[object Uint8Array]"]=pt["[object Uint8ClampedArray]"]=pt["[object Uint16Array]"]=pt["[object Uint32Array]"]=!0,pt["[object Arguments]"]=pt["[object Array]"]=pt["[object ArrayBuffer]"]=pt["[object Boolean]"]=pt["[object DataView]"]=pt["[object Date]"]=pt["[object Error]"]=pt["[object Function]"]=pt["[object Map]"]=pt["[object Number]"]=pt["[object Object]"]=pt["[object RegExp]"]=pt["[object Set]"]=pt["[object String]"]=pt["[object WeakMap]"]=!1;var vt="object"==typeof exports&&exports&&!exports.nodeType&&exports,yt=vt&&"object"==typeof module&&module&&!module.nodeType&&module,ht=yt&&yt.exports===vt&&t.process;const jt=function(){try{var t=yt&&yt.require&&yt.require("util").types;return t||ht&&ht.binding&&ht.binding("util")}catch(e){}}();var dt=jt&&jt.isTypedArray;const _t=dt?bt(dt):function(t){return l(t)&&Z(t.length)&&!!pt[s(t)]};var gt=Object.prototype.hasOwnProperty;function wt(t,e){var r=b(t),n=!r&&ut(t),o=!r&&!n&&lt(t),c=!r&&!n&&!o&&_t(t),a=r||n||o||c,u=a?function(t,e){for(var r=-1,n=Array(t);++r<t;)n[r]=e(r);return n}(t.length,String):[],i=u.length;for(var f in t)!e&&!gt.call(t,f)||a&&("length"==f||o&&("offset"==f||"parent"==f)||c&&("buffer"==f||"byteLength"==f||"byteOffset"==f)||H(f,i))||u.push(f);return u}function Ot(t,e){return function(r){return t(e(r))}}const mt=Ot(Object.keys,Object);var At=Object.prototype.hasOwnProperty;function xt(t){return tt(t)?wt(t):function(t){if(!rt(t))return mt(t);var e=[];for(var r in Object(t))At.call(t,r)&&"constructor"!=r&&e.push(r);return e}(t)}var St=Object.prototype.hasOwnProperty;function zt(t){if(!g(t))return function(t){var e=[];if(null!=t)for(var r in Object(t))e.push(r);return e}(t);var e=rt(t),r=[];for(var n in t)("constructor"!=n||!e&&St.call(t,n))&&r.push(n);return r}function Pt(t){return tt(t)?wt(t,!0):zt(t)}var Et=/\.|\[(?:[^[\]]*|(["'])(?:(?!\1)[^\\]|\\.)*?\1)\]/,Tt=/^\w*$/;const It=N(Object,"create");var Ft=Object.prototype.hasOwnProperty;var Mt=Object.prototype.hasOwnProperty;function Ut(t){var e=-1,r=null==t?0:t.length;for(this.clear();++e<r;){var n=t[e];this.set(n[0],n[1])}}function $t(t,e){for(var r=t.length;r--;)if(K(t[r][0],e))return r;return-1}Ut.prototype.clear=function(){this.__data__=It?It(null):{},this.size=0},Ut.prototype.delete=function(t){var e=this.has(t)&&delete this.__data__[t];return this.size-=e?1:0,e},Ut.prototype.get=function(t){var e=this.__data__;if(It){var r=e[t];return"__lodash_hash_undefined__"===r?void 0:r}return Ft.call(e,t)?e[t]:void 0},Ut.prototype.has=function(t){var e=this.__data__;return It?void 0!==e[t]:Mt.call(e,t)},Ut.prototype.set=function(t,e){var r=this.__data__;return this.size+=this.has(t)?0:1,r[t]=It&&void 0===e?"__lodash_hash_undefined__":e,this};var kt=Array.prototype.splice;function Bt(t){var e=-1,r=null==t?0:t.length;for(this.clear();++e<r;){var n=t[e];this.set(n[0],n[1])}}Bt.prototype.clear=function(){this.__data__=[],this.size=0},Bt.prototype.delete=function(t){var e=this.__data__,r=$t(e,t);return!(r<0)&&(r==e.length-1?e.pop():kt.call(e,r,1),--this.size,!0)},Bt.prototype.get=function(t){var e=this.__data__,r=$t(e,t);return r<0?void 0:e[r][1]},Bt.prototype.has=function(t){return $t(this.__data__,t)>-1},Bt.prototype.set=function(t,e){var r=this.__data__,n=$t(r,t);return n<0?(++this.size,r.push([t,e])):r[n][1]=e,this};const Dt=N(r,"Map");function Nt(t,e){var r,n,o=t.__data__;return("string"==(n=typeof(r=e))||"number"==n||"symbol"==n||"boolean"==n?"__proto__"!==r:null===r)?o["string"==typeof e?"string":"hash"]:o.map}function Ct(t){var e=-1,r=null==t?0:t.length;for(this.clear();++e<r;){var n=t[e];this.set(n[0],n[1])}}Ct.prototype.clear=function(){this.size=0,this.__data__={hash:new Ut,map:new(Dt||Bt),string:new Ut}},Ct.prototype.delete=function(t){var e=Nt(this,t).delete(t);return this.size-=e?1:0,e},Ct.prototype.get=function(t){return Nt(this,t).get(t)},Ct.prototype.has=function(t){return Nt(this,t).has(t)},Ct.prototype.set=function(t,e){var r=Nt(this,t),n=r.size;return r.set(t,e),this.size+=r.size==n?0:1,this};function Lt(t,e){if("function"!=typeof t||null!=e&&"function"!=typeof e)throw new TypeError("Expected a function");var r=function(){var n=arguments,o=e?e.apply(this,n):n[0],c=r.cache;if(c.has(o))return c.get(o);var a=t.apply(this,n);return r.cache=c.set(o,a)||c,a};return r.cache=new(Lt.Cache||Ct),r}Lt.Cache=Ct;var Vt,Wt,Rt,qt=/[^.[\]]+|\[(?:(-?\d+(?:\.\d+)?)|(["'])((?:(?!\2)[^\\]|\\.)*?)\2)\]|(?=(?:\.|\[\])(?:\.|\[\]|$))/g,Gt=/\\(\\)?/g,Ht=(Vt=function(t){var e=[];return 46===t.charCodeAt(0)&&e.push(""),t.replace(qt,(function(t,r,n,o){e.push(n?o.replace(Gt,"$1"):r||t)})),e},Wt=Lt(Vt,(function(t){return 500===Rt.size&&Rt.clear(),t})),Rt=Wt.cache,Wt);const Jt=Ht;function Kt(t,e){return b(t)?t:function(t,e){if(b(t))return!1;var r=typeof t;return!("number"!=r&&"symbol"!=r&&"boolean"!=r&&null!=t&&!p(t))||Tt.test(t)||!Et.test(t)||null!=e&&t in Object(e)}(t,e)?[t]:Jt(function(t){return null==t?"":h(t)}(t))}function Qt(t){if("string"==typeof t||p(t))return t;var e=t+"";return"0"==e&&1/t==-Infinity?"-0":e}function Xt(t,e,r){var n=null==t?void 0:function(t,e){for(var r=0,n=(e=Kt(e,t)).length;null!=t&&r<n;)t=t[Qt(e[r++])];return r&&r==n?t:void 0}(t,e);return void 0===n?r:n}function Yt(t,e){for(var r=-1,n=e.length,o=t.length;++r<n;)t[o+r]=e[r];return t}const Zt=Ot(Object.getPrototypeOf,Object);function te(){if(!arguments.length)return[];var t=arguments[0];return b(t)?t:[t]}function ee(t){var e=this.__data__=new Bt(t);this.size=e.size}ee.prototype.clear=function(){this.__data__=new Bt,this.size=0},ee.prototype.delete=function(t){var e=this.__data__,r=e.delete(t);return this.size=e.size,r},ee.prototype.get=function(t){return this.__data__.get(t)},ee.prototype.has=function(t){return this.__data__.has(t)},ee.prototype.set=function(t,e){var r=this.__data__;if(r instanceof Bt){var n=r.__data__;if(!Dt||n.length<199)return n.push([t,e]),this.size=++r.size,this;r=this.__data__=new Ct(n)}return r.set(t,e),this.size=r.size,this};var re="object"==typeof exports&&exports&&!exports.nodeType&&exports,ne=re&&"object"==typeof module&&module&&!module.nodeType&&module,oe=ne&&ne.exports===re?r.Buffer:void 0,ce=oe?oe.allocUnsafe:void 0;function ae(){return[]}var ue=Object.prototype.propertyIsEnumerable,ie=Object.getOwnPropertySymbols,fe=ie?function(t){return null==t?[]:(t=Object(t),function(t,e){for(var r=-1,n=null==t?0:t.length,o=0,c=[];++r<n;){var a=t[r];e(a,r,t)&&(c[o++]=a)}return c}(ie(t),(function(e){return ue.call(t,e)})))}:ae;const se=fe;var le=Object.getOwnPropertySymbols?function(t){for(var e=[];t;)Yt(e,se(t)),t=Zt(t);return e}:ae;const pe=le;function be(t,e,r){var n=e(t);return b(t)?n:Yt(n,r(t))}function ve(t){return be(t,xt,se)}function ye(t){return be(t,Pt,pe)}const he=N(r,"DataView");const je=N(r,"Promise");const de=N(r,"Set");var _e=I(he),ge=I(Dt),we=I(je),Oe=I(de),me=I(C),Ae=s;(he&&"[object DataView]"!=Ae(new he(new ArrayBuffer(1)))||Dt&&"[object Map]"!=Ae(new Dt)||je&&"[object Promise]"!=Ae(je.resolve())||de&&"[object Set]"!=Ae(new de)||C&&"[object WeakMap]"!=Ae(new C))&&(Ae=function(t){var e=s(t),r="[object Object]"==e?t.constructor:void 0,n=r?I(r):"";if(n)switch(n){case _e:return"[object DataView]";case ge:return"[object Map]";case we:return"[object Promise]";case Oe:return"[object Set]";case me:return"[object WeakMap]"}return e});const xe=Ae;var Se=Object.prototype.hasOwnProperty;const ze=r.Uint8Array;function Pe(t){var e=new t.constructor(t.byteLength);return new ze(e).set(new ze(t)),e}var Ee=/\w*$/;var Te=n?n.prototype:void 0,Ie=Te?Te.valueOf:void 0;function Fe(t,e,r){var n,o=t.constructor;switch(e){case"[object ArrayBuffer]":return Pe(t);case"[object Boolean]":case"[object Date]":return new o(+t);case"[object DataView]":return function(t,e){var r=e?Pe(t.buffer):t.buffer;return new t.constructor(r,t.byteOffset,t.byteLength)}(t,r);case"[object Float32Array]":case"[object Float64Array]":case"[object Int8Array]":case"[object Int16Array]":case"[object Int32Array]":case"[object Uint8Array]":case"[object Uint8ClampedArray]":case"[object Uint16Array]":case"[object Uint32Array]":return function(t,e){var r=e?Pe(t.buffer):t.buffer;return new t.constructor(r,t.byteOffset,t.length)}(t,r);case"[object Map]":case"[object Set]":return new o;case"[object Number]":case"[object String]":return new o(t);case"[object RegExp]":return function(t){var e=new t.constructor(t.source,Ee.exec(t));return e.lastIndex=t.lastIndex,e}(t);case"[object Symbol]":return n=t,Ie?Object(Ie.call(n)):{}}}var Me=jt&&jt.isMap;const Ue=Me?bt(Me):function(t){return l(t)&&"[object Map]"==xe(t)};var $e=jt&&jt.isSet;const ke=$e?bt($e):function(t){return l(t)&&"[object Set]"==xe(t)};var Be={};function De(t,e,r,n,o,c){var a,u=1&e,i=2&e,f=4&e;if(r&&(a=o?r(t,n,o,c):r(t)),void 0!==a)return a;if(!g(t))return t;var s=b(t);if(s){if(a=function(t){var e=t.length,r=new t.constructor(e);return e&&"string"==typeof t[0]&&Se.call(t,"index")&&(r.index=t.index,r.input=t.input),r}(t),!u)return function(t,e){var r=-1,n=t.length;for(e||(e=Array(n));++r<n;)e[r]=t[r];return e}(t,a)}else{var l=xe(t),p="[object Function]"==l||"[object GeneratorFunction]"==l;if(lt(t))return function(t,e){if(e)return t.slice();var r=t.length,n=ce?ce(r):new t.constructor(r);return t.copy(n),n}(t,u);if("[object Object]"==l||"[object Arguments]"==l||p&&!o){if(a=i||p?{}:function(t){return"function"!=typeof t.constructor||rt(t)?{}:W(Zt(t))}(t),!u)return i?function(t,e){return Y(t,pe(t),e)}(t,function(t,e){return t&&Y(e,Pt(e),t)}(a,t)):function(t,e){return Y(t,se(t),e)}(t,function(t,e){return t&&Y(e,xt(e),t)}(a,t))}else{if(!Be[l])return o?t:{};a=Fe(t,l,u)}}c||(c=new ee);var v=c.get(t);if(v)return v;c.set(t,a),ke(t)?t.forEach((function(n){a.add(De(n,e,r,n,t,c))})):Ue(t)&&t.forEach((function(n,o){a.set(o,De(n,e,r,o,t,c))}));var y=s?void 0:(f?i?ye:ve:i?Pt:xt)(t);return function(t,e){for(var r=-1,n=null==t?0:t.length;++r<n&&!1!==e(t[r],r,t););}(y||t,(function(n,o){y&&(n=t[o=n]),X(a,o,De(n,e,r,o,t,c))})),a}Be["[object Arguments]"]=Be["[object Array]"]=Be["[object ArrayBuffer]"]=Be["[object DataView]"]=Be["[object Boolean]"]=Be["[object Date]"]=Be["[object Float32Array]"]=Be["[object Float64Array]"]=Be["[object Int8Array]"]=Be["[object Int16Array]"]=Be["[object Int32Array]"]=Be["[object Map]"]=Be["[object Number]"]=Be["[object Object]"]=Be["[object RegExp]"]=Be["[object Set]"]=Be["[object String]"]=Be["[object Symbol]"]=Be["[object Uint8Array]"]=Be["[object Uint8ClampedArray]"]=Be["[object Uint16Array]"]=Be["[object Uint32Array]"]=!0,Be["[object Error]"]=Be["[object Function]"]=Be["[object WeakMap]"]=!1;function Ne(t){return De(t,4)}function Ce(t){var e=-1,r=null==t?0:t.length;for(this.__data__=new Ct;++e<r;)this.add(t[e])}function Le(t,e){for(var r=-1,n=null==t?0:t.length;++r<n;)if(e(t[r],r,t))return!0;return!1}function Ve(t,e){return t.has(e)}Ce.prototype.add=Ce.prototype.push=function(t){return this.__data__.set(t,"__lodash_hash_undefined__"),this},Ce.prototype.has=function(t){return this.__data__.has(t)};function We(t,e,r,n,o,c){var a=1&r,u=t.length,i=e.length;if(u!=i&&!(a&&i>u))return!1;var f=c.get(t),s=c.get(e);if(f&&s)return f==e&&s==t;var l=-1,p=!0,b=2&r?new Ce:void 0;for(c.set(t,e),c.set(e,t);++l<u;){var v=t[l],y=e[l];if(n)var h=a?n(y,v,l,e,t,c):n(v,y,l,t,e,c);if(void 0!==h){if(h)continue;p=!1;break}if(b){if(!Le(e,(function(t,e){if(!Ve(b,e)&&(v===t||o(v,t,r,n,c)))return b.push(e)}))){p=!1;break}}else if(v!==y&&!o(v,y,r,n,c)){p=!1;break}}return c.delete(t),c.delete(e),p}function Re(t){var e=-1,r=Array(t.size);return t.forEach((function(t,n){r[++e]=[n,t]})),r}function qe(t){var e=-1,r=Array(t.size);return t.forEach((function(t){r[++e]=t})),r}var Ge=n?n.prototype:void 0,He=Ge?Ge.valueOf:void 0;var Je=Object.prototype.hasOwnProperty;var Ke="[object Object]",Qe=Object.prototype.hasOwnProperty;function Xe(t,e,r,n,o,c){var a=b(t),u=b(e),i=a?"[object Array]":xe(t),f=u?"[object Array]":xe(e),s=(i="[object Arguments]"==i?Ke:i)==Ke,l=(f="[object Arguments]"==f?Ke:f)==Ke,p=i==f;if(p&&lt(t)){if(!lt(e))return!1;a=!0,s=!1}if(p&&!s)return c||(c=new ee),a||_t(t)?We(t,e,r,n,o,c):function(t,e,r,n,o,c,a){switch(r){case"[object DataView]":if(t.byteLength!=e.byteLength||t.byteOffset!=e.byteOffset)return!1;t=t.buffer,e=e.buffer;case"[object ArrayBuffer]":return!(t.byteLength!=e.byteLength||!c(new ze(t),new ze(e)));case"[object Boolean]":case"[object Date]":case"[object Number]":return K(+t,+e);case"[object Error]":return t.name==e.name&&t.message==e.message;case"[object RegExp]":case"[object String]":return t==e+"";case"[object Map]":var u=Re;case"[object Set]":var i=1&n;if(u||(u=qe),t.size!=e.size&&!i)return!1;var f=a.get(t);if(f)return f==e;n|=2,a.set(t,e);var s=We(u(t),u(e),n,o,c,a);return a.delete(t),s;case"[object Symbol]":if(He)return He.call(t)==He.call(e)}return!1}(t,e,i,r,n,o,c);if(!(1&r)){var v=s&&Qe.call(t,"__wrapped__"),y=l&&Qe.call(e,"__wrapped__");if(v||y){var h=v?t.value():t,j=y?e.value():e;return c||(c=new ee),o(h,j,r,n,c)}}return!!p&&(c||(c=new ee),function(t,e,r,n,o,c){var a=1&r,u=ve(t),i=u.length;if(i!=ve(e).length&&!a)return!1;for(var f=i;f--;){var s=u[f];if(!(a?s in e:Je.call(e,s)))return!1}var l=c.get(t),p=c.get(e);if(l&&p)return l==e&&p==t;var b=!0;c.set(t,e),c.set(e,t);for(var v=a;++f<i;){var y=t[s=u[f]],h=e[s];if(n)var j=a?n(h,y,s,e,t,c):n(y,h,s,t,e,c);if(!(void 0===j?y===h||o(y,h,r,n,c):j)){b=!1;break}v||(v="constructor"==s)}if(b&&!v){var d=t.constructor,_=e.constructor;d==_||!("constructor"in t)||!("constructor"in e)||"function"==typeof d&&d instanceof d&&"function"==typeof _&&_ instanceof _||(b=!1)}return c.delete(t),c.delete(e),b}(t,e,r,n,o,c))}function Ye(t,e,r,n,o){return t===e||(null==t||null==e||!l(t)&&!l(e)?t!=t&&e!=e:Xe(t,e,r,n,Ye,o))}const Ze=function(){return r.Date.now()};var tr=Math.max,er=Math.min;function rr(t,e,r){var n,o,c,a,u,i,f=0,s=!1,l=!1,p=!0;if("function"!=typeof t)throw new TypeError("Expected a function");function b(e){var r=n,c=o;return n=o=void 0,f=e,a=t.apply(c,r)}function v(t){return f=t,u=setTimeout(h,e),s?b(t):a}function y(t){var r=t-i;return void 0===i||r>=e||r<0||l&&t-f>=c}function h(){var t=Ze();if(y(t))return j(t);u=setTimeout(h,function(t){var r=e-(t-i);return l?er(r,c-(t-f)):r}(t))}function j(t){return u=void 0,p&&n?b(t):(n=o=void 0,a)}function d(){var t=Ze(),r=y(t);if(n=arguments,o=this,i=t,r){if(void 0===u)return v(i);if(l)return clearTimeout(u),u=setTimeout(h,e),b(i)}return void 0===u&&(u=setTimeout(h,e)),a}return e=x(e)||0,g(r)&&(s=!!r.leading,c=(l="maxWait"in r)?tr(x(r.maxWait)||0,e):c,p="trailing"in r?!!r.trailing:p),d.cancel=function(){void 0!==u&&clearTimeout(u),f=0,n=i=o=u=void 0},d.flush=function(){return void 0===u?a:j(Ze())},d}function nr(t){for(var e=-1,r=null==t?0:t.length,n={};++e<r;){var o=t[e];n[o[0]]=o[1]}return n}function or(t,e){return Ye(t,e)}function cr(t){return null==t}function ar(t,e,r){return null==t?t:function(t,e,r,n){if(!g(t))return t;for(var o=-1,c=(e=Kt(e,t)).length,a=c-1,u=t;null!=u&&++o<c;){var i=Qt(e[o]),f=r;if("__proto__"===i||"constructor"===i||"prototype"===i)return t;if(o!=a){var s=u[i];void 0===(f=n?n(s,i,u):void 0)&&(f=g(s)?s:H(e[o+1])?[]:{})}X(u,i,f),u=u[i]}return t}(t,e,r)}export{Ne as a,or as b,te as c,rr as d,nr as f,Xt as g,cr as i,ar as s};