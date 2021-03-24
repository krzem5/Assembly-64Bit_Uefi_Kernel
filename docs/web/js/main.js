window.onload=()=>{
	let BASE_URL="https://github.com/Krzem5/Assembly-64Bit_Uefi_Kernel/";
	let nve=document.querySelector(".path");
	let ghe=document.querySelector(".gh");
	window._show_dir=(p)=>{
		location.hash="D"+p;
		let tp="";
		nve.innerHTML=p.split("/").map((e,i,a)=>{
			tp+=(tp?"/":"")+e;
			return `<span class="${(i?"dir":"root")+(i<a.length-1?"":" e")}"${(i<a.length-1?"onclick=\"window._show_dir('"+tp+"')\"":"")}>${e}</span>`
		}).join("/");
		ghe.href=BASE_URL+`tree/main/${p}`;
		console.log(p,window._data.dirs[p]);
	}
	window._show=(p)=>{
		location.hash="F"+p;
		let tp="";
		nve.innerHTML=p.split("/").map((e,i,a)=>{
			tp+=(tp?"/":"")+e;
			return `<span class="${(i<a.length-1?(i?"dir":"root"):(e[e.length-1]=="c"?"c":e[e.length-1]=="h"?"h":"asm")+" e")}"${(i<a.length-1?"onclick=\"window._show_dir('"+tp+"')\"":"")}>${e}</span>`
		}).join("/");
		ghe.href=BASE_URL+`blob/main/${p}`;
		console.log(p,window._data.files[p]);
	}
	if (location.hash.length>2){
		window[(location.hash[1]=="D"?"_show_dir":"_show")](location.hash.substring(2));
	}
}
