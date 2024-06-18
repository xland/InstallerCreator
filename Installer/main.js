app.initFont(["Microsoft YaHei","iconfont.ttf"]);
let win = new Win("我的窗口", 800, 600); //默认为透明窗口
let bannerArr = [];
let getBgRect = () => {
    let rect = Rect.newXYWH(50, 50, 700, 500); //550
    rect.setColor(0xFFe6f4ff);
    rect.setShadowSize(30);
    rect.setBorderRadius(3, 3, 3, 3)
    return rect;
}
let getTitleDiv = () => {
    let titleDiv = Div.newLTRB(50, 50, 750, 80)
    titleDiv.setBorderRadius(3, 3, 0, 0)
    titleDiv.setAlign(1, 0);
    titleDiv.setIndent(0, 8)
    titleDiv.setColor(0xFFe6f4ff);
    titleDiv.setTextColor(0xFF888888)
    titleDiv.setFontSize(12);
    titleDiv.setFontFamily("Microsoft YaHei");
    titleDiv.setText("软件名称 - 安装程序")
    return titleDiv;
}
let getCloseDiv = () => {
    let closeDiv = Div.newLTRB(715, 50, 750, 80);
    closeDiv.setBorderRadius(0, 3, 0, 0)
    closeDiv.setColor(0x00000000);
    closeDiv.setFontFamily("iconfont.ttf");
    closeDiv.setAlign(1, 1);
    closeDiv.setIcon(0xE6E6);
    closeDiv.setFontSize(14);
    closeDiv.setTextColor(0x88000000);
    closeDiv.onMouseEnter(() => {
        closeDiv.setColor(0xFFE81123);
        closeDiv.setTextColor(0xFFFFFFFF);
        win.refresh();
    });
    closeDiv.onMouseLeave(() => {
        closeDiv.setColor(0x00000000);
        closeDiv.setTextColor(0x88000000);
        win.refresh();
    });
    closeDiv.onMouseDown(() => {
        win.close();
        console.log("down");
    })
    return closeDiv;
}
let getMinimizeDiv = () => {
    let minimizeDiv = Div.newLTRB(680, 50, 715, 80);
    minimizeDiv.setColor(0x00000000);
    minimizeDiv.setFontFamily("iconfont.ttf");
    minimizeDiv.setAlign(1, 1);
    minimizeDiv.setIcon(0xE6E7);
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
    return minimizeDiv;
}
let initBannerArr = () => {
    for (let i = 0; i < 3; i++) {
        let banner = Img.newXYWH(50, 80, 700, 350);
        banner.setSrc(`banner${i}.png`);
        if (i != 0) {
            banner.setAlpha(0);
        }
        bannerArr.push(banner);
    }
}
let getStartBtn = () => {
    let startBtn = Div.newXYWH(340, 410, 120, 40);
    startBtn.setBorderRadius(6, 6, 6, 6)
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
    return startBtn;
}
let setWinDragPath = () => {
    let path = new Path();
    path.addRect(Rect.newLTRB(50, 50, 680, 80));
    path.addRect(Rect.newLTRB(50, 80, 700, 350));
    win.setCaptionPath(path);
}
let setBannerAnimation = () => {
    let curIndex = 0;
    let span = 1600;
    let alpha = 1;
    let intervalSpan = 20;
    win.setInterval(() => {
        if (span > 0) {
            span -= intervalSpan; //等待
            return;
        }
        let nextIndex = curIndex + 1;
        if (nextIndex >= bannerArr.length) {
            nextIndex = 0;
        }
        alpha -= 0.02;
        bannerArr[curIndex].setAlpha(alpha);
        bannerArr[nextIndex].setAlpha(1 - alpha);
        if (alpha <= 0) {
            alpha = 1;
            span = 1600;
            curIndex = nextIndex;
            for (let i = 0; i < bannerArr.length; i++) {
                if (i != curIndex) {
                    bannerArr[i].setAlpha(0);
                }
            }
        }
        win.refresh();
    }, intervalSpan)
}
let getLicenceDiv = () => {
    let divCheckbox = Div.newXYWH(70, 480, 16, 18);
    divCheckbox.setFontFamily("iconfont.ttf")
    divCheckbox.setColor(0x00000000);
    divCheckbox.setTextColor(0xFF888888);
    divCheckbox.setIcon(0xE608);
    let divText = Div.newXYWH(86, 480, 96, 18);
    divText.setColor(0x00000000);
    divText.setTextColor(0xFF888888);
    divText.setFontFamily("Microsoft YaHei");
    divText.setText("您已阅读并同意")
    let divLink = Div.newXYWH(178, 480, 56, 18);
    divLink.setColor(0x00000000);
    divLink.setTextColor(0xFF888888);
    divLink.setFontFamily("Microsoft YaHei");
    divLink.setText("用户协议")
    divLink.setDecoration(1, 0xFF0969DA);
    divLink.onMouseEnter(() => {
        app.setCursor("pointer");
    })
    divLink.onMouseLeave(() => {
        app.setCursor("default");
    })
    divLink.onMouseDown(() => {

    })
    let switchCheck = () => {
        if (divCheckbox.getIcon() === 0xE608) {
            divCheckbox.setIcon(0xE609)
        } else {
            divCheckbox.setIcon(0xE608)
        }
        win.refresh();
    }
    divCheckbox.onMouseDown(switchCheck);
    divText.onMouseDown(switchCheck);
    return [divCheckbox,divText,divLink];
}
let start = () => {
    initBannerArr();
    setWinDragPath();
    win.addElement([
        getBgRect(),
        getTitleDiv(),
        getCloseDiv(),
        getMinimizeDiv(),
        ...bannerArr,
        getStartBtn(),
        ...getLicenceDiv()
    ]);
    win.show();
    setBannerAnimation();
    globalThis.win = win;
}
start();