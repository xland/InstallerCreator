app.initFont(["Microsoft YaHei","iconfont.ttf"]);
let win = new Win("我的窗口", 800, 600); //默认为透明窗口

let closeDiv = Div.newLTRB(715, 50, 750, 80);
closeDiv.setColor(0x00000000);
closeDiv.setFontFamily("iconfont.ttf");
closeDiv.setAlign(1, 1);
closeDiv.setText(0xE6E6);
closeDiv.setFontSize(14);
closeDiv.setTextColor(0x88000000);
closeDiv.onMouseEnter(() => {
    closeDiv.setColor(0x11000000);
    win.refresh();
});
closeDiv.onMouseLeave(() => {
    closeDiv.setColor(0x00000000);
    win.refresh();
});
closeDiv.onMouseDown(() => {
    win.close();
    console.log("down");
})


let minimizeDiv = Div.newLTRB(680, 50, 715, 80);
minimizeDiv.setColor(0x00000000);
minimizeDiv.setFontFamily("iconfont.ttf");
minimizeDiv.setAlign(1, 1);
minimizeDiv.setText(0xE6E7);
minimizeDiv.setFontSize(14);
minimizeDiv.setTextColor(0x88000000);
minimizeDiv.onMouseEnter(() => {
    minimizeDiv.setColor(0x11000000);
    win.refresh();
});
minimizeDiv.onMouseLeave(() => {
    minimizeDiv.setColor(0x00000000);
    win.refresh();
});
minimizeDiv.onMouseDown(() => {
    win.minimize();
})

let div = Div.newLTRB(50, 80, 750, 550);
div.setColor(0xFFE6E6E6);

let shadow = new Shadow();
shadow.setAmbientColor(0x33000000);
shadow.setSpotColor(0x00000000);
shadow.setShadowSize(30);
shadow.addRect(Rect.newXYWH(50, 50, 700, 500))

let banner1 = Img.newXYWH(50, 80, 700, 350);
banner1.setSrc("banner1.png");
let banner2 = Img.newXYWH(50, 80, 700, 350);
banner2.setSrc("banner2.png");
banner2.setAlpha(0);
let banner3 = Img.newXYWH(50, 80, 700, 350);
banner3.setSrc("banner3.png");
banner3.setAlpha(0);
let bannerArr = [banner1, banner2, banner3];

let startBtn = Div.newXYWH(340, 410, 120, 40);
startBtn.setBorderRadius(6,6,6,6)
startBtn.setAlign(1, 1);
startBtn.setColor(0xFF4096ff);
startBtn.setTextColor(0xFFFFFFFF)
startBtn.setFontSize(16);
startBtn.setFontFamily("Microsoft YaHei");
startBtn.setText("开始安装")
startBtn.onMouseEnter(() => {
    app.setCursor("pointer");
    startBtn.setColor(0xFF1677ff);
    win.refresh();
})
startBtn.onMouseLeave(() => {
    app.setCursor("default");
    startBtn.setColor(0xFF4096ff);
    win.refresh();
})
startBtn.onMouseDown(() => {

})

let titleDiv = Div.newLTRB(50, 50, 750, 80)
titleDiv.setAlign(1, 0);
titleDiv.setIndent(0,8)
titleDiv.setColor(0xFFf6f6f6);
titleDiv.setTextColor(0xFF888888)
titleDiv.setFontSize(12);
titleDiv.setFontFamily("Microsoft YaHei");
titleDiv.setText("软件名称 - 安装程序")

win.addElement([shadow, div, ...bannerArr, titleDiv,closeDiv, minimizeDiv,startBtn]);


let path = new Path();
path.addRect(Rect.newLTRB(50, 50, 680, 80));
path.addRect(Rect.newLTRB(50, 80, 700, 350));
win.setCaptionPath(path);

win.show();


let curIndex = 0;
let span = 3000;
let alpha = 1;
win.setInterval(() => {
    if (span > 0) {
        span -= 20; //等待
        return;
    }
    let nextIndex = curIndex + 1;
    if (nextIndex >= bannerArr.length) {
        nextIndex = 0;
    }
    alpha -= 0.02;
    bannerArr[curIndex].setAlpha(alpha);
    bannerArr[nextIndex].setAlpha(1-alpha);
    if (alpha <= 0) {
        alpha = 1;
        span = 3000;
        curIndex = nextIndex;
        for (let i = 0; i < bannerArr.length; i++) {
            if (i != curIndex) {
                bannerArr[i].setAlpha(0);
            }
        }
    }
    win.refresh();
},20)

globalThis.win = win;