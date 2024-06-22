//app.initFont(["Microsoft YaHei", "iconfont.ttf"]);
//let str = app.getKnownFolder("B4BFCC3A-DB2C-424C-B029-7FE99A87C641");

app.initFileFont("iconfont.ttf","iconFont",1); //app ready之前完成此工作
let windowTitle = "软件名称 - 安装程序";
let bannerArr = [];
let win;
let getBgRect = () => {
    let bgRect = Rect.newXYWH(50, 50, 700, 450); //550
    bgRect.setColor(0xffe6f4ff);
    bgRect.setShadowSize(30);
    bgRect.setBorderRadius(3, 3, 3, 3);
    return bgRect;
};
let getTitleDiv = () => {
    let text = new Text();
    text.setColor(0xff888888);
    text.setFontSize(12);
    text.setText(windowTitle);
    let rect = new Rect();
    rect.setLTRB(50, 50, 750, 80);
    rect.setBorderRadius(3, 3, 0, 0);
    rect.setColor(0xffe6f4ff);
    let titleDiv = new Div();
    titleDiv.setText(text);
    titleDiv.setRect(rect);
    titleDiv.setAlign(1, 0);
    titleDiv.setIndent(0, 8);
    return titleDiv;
};
let getCloseDiv = () => {
    let closeDiv = Div.newLTRB(715, 50, 750, 80);
    closeDiv.setBorderRadius(0, 3, 0, 0);
    closeDiv.setColor(0x00000000);
    closeDiv.setFontFamily("iconfont.ttf");
    closeDiv.setAlign(1, 1);
    closeDiv.setIcon(0xe6e6);
    closeDiv.setFontSize(14);
    closeDiv.setTextColor(0x88000000);
    closeDiv.onMouseEnter(() => {
    closeDiv.setColor(0xffe81123);
    closeDiv.setTextColor(0xffffffff);
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
    });
    return closeDiv;
};
let getMinimizeDiv = () => {
    let minimizeDiv = Div.newLTRB(680, 50, 715, 80);
    minimizeDiv.setColor(0x00000000);
    minimizeDiv.setFontFamily("iconfont.ttf");
    minimizeDiv.setAlign(1, 1);
    minimizeDiv.setIcon(0xe6e7);
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
  });
  return minimizeDiv;
};
let initBannerArr = () => {
  for (let i = 0; i < 3; i++) {
    let banner = Img.newXYWH(50, 80, 700, 350);
    banner.setSrc(`banner${i}.png`);
    if (i != 0) {
      banner.setAlpha(0);
    }
    bannerArr.push(banner);
  }
};
let getStartBtn = () => {
  let startBtn = Div.newXYWH(340, 410, 120, 40);
  startBtn.setBorderRadius(6, 6, 6, 6);
  startBtn.setAlign(1, 1);
  startBtn.setColor(0xff4096ff);
  startBtn.setTextColor(0xffffffff);
  startBtn.setFontSize(15);
  startBtn.setFontFamily("Microsoft YaHei");
  startBtn.setText("开始安装");
  startBtn.onMouseEnter(() => {
    app.setCursor("pointer");
    startBtn.setColor(0xff1677ff);
    win.refresh();
  });
  startBtn.onMouseLeave(() => {
    app.setCursor("default");
    startBtn.setColor(0xff4096ff);
    win.refresh();
  });
    startBtn.onMouseDown(() => {
        let str = win.openPathSelectDialog();
        console.log(str);

      startBtn.setColor(0xFF0958d9);
      win.refresh();
  })
  startBtn.onMouseUp(() => {
      startBtn.setColor(0xFF1677ff);
      win.refresh();
  })
  return startBtn;
};
let setWinDragPath = () => {
  let path = new Path();
  path.addRect(Rect.newLTRB(50, 50, 680, 80));
  path.addRect(Rect.newLTRB(50, 80, 700, 350));
  win.setCaptionPath(path);
};
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
  }, intervalSpan);
};
let getLicenceDiv = () => {
    let divCheckbox = Div.newXYWH(70, 460, 16, 18);
    divCheckbox.setFontFamily("iconfont.ttf");
    divCheckbox.setColor(0x00000000);
    divCheckbox.setTextColor(0xff666666);
    divCheckbox.setIcon(0xe608);
    let divText = Div.newXYWH(86, 460, 96, 18);
    divText.setColor(0x00000000);
    divText.setTextColor(0xff666666);
    //divText.setFontSize(26);
    divText.setFontFamily("Microsoft YaHei");
    divText.setText("您已阅读并同意");
    let divLink = Div.newXYWH(178, 460, 56, 18);
    divLink.setColor(0x00000000);
    divLink.setTextColor(0xff666666);
    divLink.setFontFamily("Microsoft YaHei");
    divLink.setText("用户协议");
    divLink.setDecoration(1, 0xff0969da);
    divLink.onMouseEnter(() => {
        app.setCursor("pointer");
    });
    divLink.onMouseLeave(() => {
        app.setCursor("default");
    });
    divLink.onMouseDown(() => {
        app.openUrlByDefaultBrowser("https://www.baidu.com")
    });
    let switchCheck = () => {
        if (divCheckbox.getIcon() === 0xe608) {
            divCheckbox.setIcon(0xe609);
        } else {
            divCheckbox.setIcon(0xe608);
        }
        win.refresh();
    };
    divCheckbox.onMouseDown(switchCheck);
    divText.onMouseDown(switchCheck);
    return [divCheckbox, divText, divLink];
};
let getCustomizeBtn = () => {
    let divText = Div.newLTRB(650, 460, 716, 478);
    divText.setColor(0x00000000);
    divText.setTextColor(0xff666666);
    divText.setFontFamily("Microsoft YaHei");
    divText.setText("自定义安装");
    let divIcon = Div.newXYWH(716, 462, 15, 15);
    divIcon.setFontFamily("iconfont.ttf");
    divIcon.setColor(0x00000000);
    divIcon.setTextColor(0xff666666);
    divIcon.setIcon(0xe68b);
    let mouseDown = () => {
        if (divIcon.getIcon() === 0xe68b) {
            appendCustomizeElements();
            divIcon.setIcon(0xe691);
            bgRect.setXYWH(50, 50, 700, 500);
        } else {
            removeCustomizeElements();
            divIcon.setIcon(0xe68b);
            bgRect.setXYWH(50, 50, 700, 450);
        }
        win.refresh();
    }
    divText.onMouseDown(mouseDown);
    divIcon.onMouseDown(mouseDown)
    return [divText, divIcon];
}
let appendCustomizeElements = () => {
    let input = Input.newXYWH(70, 496, 230, 26);
    input.setId("input")
    input.setFontFamily("Microsoft YaHei");
    input.setColor(0xFFFFFFFF);
    input.setTextColor(0xff666666);
    input.setText("请选择安装路径");
    input.setAlign(1, 0);
    input.setIndent(0, 6);
    

    let btn = Div.newXYWH(300, 496, 80, 26);
    btn.setId("selectPath")
    btn.setBorderRadius(0, 3, 3, 0);
    btn.setAlign(1, 1);
    btn.setColor(0xff4096ff);
    btn.setTextColor(0xffffffff);
    btn.setFontSize(12);
    btn.setFontFamily("Microsoft YaHei");
    btn.setText("选择路径");
    btn.onMouseEnter(() => {
        app.setCursor("pointer");
        btn.setColor(0xff1677ff);
        win.refresh();
    });
    btn.onMouseLeave(() => {
        app.setCursor("default");
        btn.setColor(0xff4096ff);
        win.refresh();
    });
    btn.onMouseDown(() => {
        btn.setColor(0xFF0958d9);
        win.refresh();
    })
    btn.onMouseUp(() => {
        btn.setColor(0xFF1677ff);
        win.refresh();
    })

    win.addElement([input,btn]);
}
let removeCustomizeElements = () => {
    win.removeElement("input");
    win.removeElement("selectPath");
}
app.ready(() => {
    win = new Win(windowTitle, 800, 600); //默认为透明窗口
    //initBannerArr();
    //setWinDragPath();
    win.addElement([getBgRect(), getTitleDiv()]);
    win.show();
    //setBannerAnimation();
    globalThis.win = win;
})

