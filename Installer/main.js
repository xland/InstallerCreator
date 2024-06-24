//app.initFont(["Microsoft YaHei", "iconfont.ttf"]);
//let str = app.getKnownFolder("B4BFCC3A-DB2C-424C-B029-7FE99A87C641");

app.initFileFont("iconfont.ttf","iconFont",1); //app ready之前完成此工作
let windowTitle = "软件名称 - 安装程序";
let bannerArr = [];
let win;
let setWinDragPath = () => {
    let path = new Path();
    path.addRect(Rect.newLTRB(50, 50, 680, 80));
    path.addRect(Rect.newLTRB(50, 80, 700, 350));
    win.setCaptionPath(path);
};
let getBgRect = () => {
    let rect = Rect.newXYWH(50, 50, 700, 450); //550
    rect.setId("bgRect")
    rect.setColor(0xffe6f4ff);
    rect.setShadowSize(30);
    rect.setBorderRadius(3, 3, 3, 3);
    return rect;
};
let getTitleDiv = () => {
    let text = new Text();
    text.setColor(0xff888888);
    text.setFontSize(12);
    text.setAlign(1, 0);
    text.setIndent(0, 8);
    text.setText(windowTitle);

    let rect = Rect.newLTRB(50, 50, 750, 80);
    rect.setBorderRadius(3, 3, 0, 0);
    rect.setColor(0xffe6f4ff);

    let titleDiv = new Div();
    titleDiv.setId("titleDiv")
    titleDiv.setText(text);
    titleDiv.setRect(rect);
    return titleDiv;
};
let getCloseDiv = () => {
    let rect = Rect.newLTRB(715, 50, 750, 80);
    rect.setBorderRadius(0, 3, 0, 0);
    rect.setColor(0x00000000);

    let icon = new Icon();
    icon.setIcon(0xe6e6);
    icon.setColor(0x88000000);
    icon.setFontSize(14);
    icon.setAlign(1, 1);

    let closeDiv = new Div();
    closeDiv.setId("closeDiv")
    closeDiv.setIcon(icon);
    closeDiv.setRect(rect);

    closeDiv.onMouseEnter(() => {
        closeDiv.rect.setColor(0xffe81123);
        closeDiv.icon.setColor(0xffffffff);
        win.refresh();
    });
    closeDiv.onMouseLeave(() => {
        closeDiv.rect.setColor(0x00000000);
        closeDiv.icon.setColor(0x88000000);
        win.refresh();
    });
    closeDiv.onMouseDown(() => {
        win.close();
        console.log("down");
    });
    return closeDiv;
};
let getMinimizeDiv = () => {
    let rect = Rect.newLTRB(680, 50, 715, 80);
    rect.setColor(0x00000000);

    let icon = new Icon();
    icon.setIcon(0xe6e7);
    icon.setColor(0x88000000);
    icon.setFontSize(14);
    icon.setAlign(1, 1);

    let minimizeDiv = new Div();
    minimizeDiv.setId("minimizeDiv")
    minimizeDiv.setIcon(icon);
    minimizeDiv.setRect(rect);

    minimizeDiv.onMouseEnter(() => {
        minimizeDiv.rect.setColor(0x11000000);
        win.refresh();
    });
    minimizeDiv.onMouseLeave(() => {
        minimizeDiv.rect.setColor(0x00000000);
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
    let rect = Rect.newXYWH(340, 410, 120, 40)
    rect.setBorderRadius(6, 6, 6, 6);
    rect.setColor(0xff4096ff);
    let text = new Text();
    text.setColor(0xffffffff);
    text.setFontSize(15);
    text.setText("开始安装");
    text.setAlign(1, 1);
    let startBtn = new Div();
    startBtn.setId("startBtn")
    startBtn.setText(text);
    startBtn.setRect(rect);
    startBtn.onMouseEnter(() => {
        app.setCursor("pointer");
        startBtn.rect.setColor(0xff1677ff);
        win.refresh();
    });
    startBtn.onMouseLeave(() => {
        app.setCursor("default");
        startBtn.rect.setColor(0xff4096ff);
        win.refresh();
    });
    startBtn.onMouseDown(() => {
        startBtn.rect.setColor(0xFF0958d9);
        win.refresh();
    })
    startBtn.onMouseUp(() => {
        startBtn.rect.setColor(0xFF1677ff);
        win.refresh();
    })
  return startBtn;
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
    let switchCheck = () => {
        let icon = win.getElement("licenceDivIcon")
        if (icon.getIcon() === 0xe608) {
            icon.setIcon(0xe609);
        } else {
            icon.setIcon(0xe608);
        }
        win.refresh();
    };
    let getCheckDiv = () => {
        let rect = Rect.newXYWH(70, 459, 16, 18);
        rect.setColor(0x00000000);
        let icon = new Icon();
        icon.setId("licenceDivIcon")
        icon.setIcon(0xe608);
        icon.setColor(0xff666666);
        icon.setAlign(1, 1);
        let div = new Div();
        div.setIcon(icon);
        div.setRect(rect);
        div.onMouseDown(switchCheck);
        return div;
    }
    let getTextDiv = () => {
        let rect = Rect.newXYWH(86, 460, 96, 18);
        rect.setColor(0x00000000);
        let text = new Text();
        text.setText("您已阅读并同意");
        text.setColor(0xff666666);
        let div = new Div();
        div.setText(text);
        div.setRect(rect);
        div.onMouseDown(switchCheck);
        return div;
    }
    let getLinkDiv = () => {
        let rect = Rect.newXYWH(178.6, 460, 56, 18);
        rect.setColor(0x00000000);
        let text = new Text();
        text.setText("用户协议");
        text.setColor(0xff666666);
        text.setDecoration(1, 0xff0969da);
        let div = new Div();
        div.setText(text);
        div.setRect(rect);
        div.onMouseEnter(() => {
            app.setCursor("pointer");
        });
        div.onMouseLeave(() => {
            app.setCursor("default");
        });
        div.onMouseDown(() => {
            app.openUrlByDefaultBrowser("https://www.baidu.com")
        });
        return div;
    }
    return [getCheckDiv(), getTextDiv(), getLinkDiv()];
};
let getCustomizeBtn = () => {
    let mouseDown = () => {
        let bgRect = win.getElement("bgRect")
        let icon = win.getElement("customizeBtnIcon")
        if (icon.getIcon() === 0xe68b) {
            appendCustomizeElements();
            icon.setIcon(0xe691);
            bgRect.setXYWH(50, 50, 700, 500);
        } else {
            removeCustomizeElements();
            icon.setIcon(0xe68b);
            bgRect.setXYWH(50, 50, 700, 450);
        }
        win.refresh();
    }
    let getTextDiv = () => {
        let rect = Rect.newLTRB(650, 460, 716, 478);
        rect.setColor(0x00000000);
        let text = new Text();
        text.setText("自定义安装");
        text.setColor(0xff666666);
        let div = new Div();
        div.setText(text);
        div.setRect(rect);
        div.onMouseDown(mouseDown);
        return div;
    }
    let getIconDiv = () => {
        let rect = Rect.newXYWH(716, 461, 15, 15);
        rect.setColor(0x00000000);
        let icon = new Icon();
        icon.setId("customizeBtnIcon")
        icon.setIcon(0xe68b);
        icon.setColor(0xff666666);
        icon.setAlign(1, 1);
        let div = new Div();
        div.setIcon(icon);
        div.setRect(rect);
        div.onMouseDown(mouseDown);
        return div;
    }
    return [getTextDiv(), getIconDiv()];
}
let appendCustomizeElements = () => {
    let getInput = () => {
        let rect = Rect.newXYWH(70, 496, 230, 26);
        rect.setColor(0xFFFFFFFF);
        rect.setBorderRadius(3, 0, 0, 3);
        let text = new Text();
        text.setId("inputText")
        text.setColor(0xffAAAAAA);
        text.setText("请选择安装路径");
        text.setFontSize(12)
        text.setAlign(1, 0);
        text.setIndent(0, 6);
        let input = new Input();
        input.setId("input")
        input.setText(text);
        input.setRect(rect);
        return input;
    }
    let getSelectBtn = () => {
        let rect = Rect.newXYWH(300, 496, 80, 26);
        rect.setColor(0xff4096ff);
        rect.setBorderRadius(0, 3, 3, 0);
        let text = new Text();
        text.setColor(0xffffffff);
        text.setFontSize(12);
        text.setText("选择路径");
        text.setAlign(1, 1);
        let btn = new Div();
        btn.setId("selectBtn")
        btn.setText(text);
        btn.setRect(rect);
        btn.onMouseEnter(() => {
            app.setCursor("pointer");
            rect.setColor(0xff1677ff);
            win.refresh();
        });
        btn.onMouseLeave(() => {
            app.setCursor("default");
            rect.setColor(0xff4096ff);
            win.refresh();
        });
        btn.onMouseDown(() => {
            let str = win.openPathSelectDialog();
            let inputText = win.getElement("inputText")
            if (str) {
                inputText.setText(str);
                inputText.setColor(0xff666666);
            } else {
                inputText.setColor(0xffAAAAAA);
            }
            win.refresh();
        })
        return btn;
    }
    let getDesktopBtn = () => {
        let switchCheck = () => {
            let icon = win.getElement("desktopIcon")
            if (icon.getIcon() === 0xe608) {
                icon.setIcon(0xe609);
            } else {
                icon.setIcon(0xe608);
            }
            win.refresh();
        };
        let getCheckDiv = () => {
            let rect = Rect.newXYWH(515, 500, 16, 18);
            rect.setColor(0x00000000);
            let icon = new Icon();
            icon.setId("desktopIcon")
            icon.setIcon(0xe608);
            icon.setColor(0xff666666);
            icon.setAlign(1, 1);
            let div = new Div();
            div.setIcon(icon);
            div.setId("desktopCheck")
            div.setRect(rect);
            div.onMouseDown(switchCheck);
            return div;
        }
        let getTextDiv = () => {
            let rect = Rect.newXYWH(530, 500, 96, 18);
            rect.setColor(0x00000000);
            let text = new Text();
            text.setText("创建桌面图标");
            text.setColor(0xff666666);
            let div = new Div();
            div.setText(text);
            div.setRect(rect);
            div.setId("desktopText")
            div.onMouseDown(switchCheck);
            return div;
        }
        return [getCheckDiv(),getTextDiv()]
    }
    let getMenuBtn = () => {
        let switchCheck = () => {
            let icon = win.getElement("menuIcon")
            if (icon.getIcon() === 0xe608) {
                icon.setIcon(0xe609);
            } else {
                icon.setIcon(0xe608);
            }
            win.refresh();
        };
        let getCheckDiv = () => {
            let rect = Rect.newXYWH(635, 500, 16, 18);
            rect.setColor(0x00000000);
            let icon = new Icon();
            icon.setId("menuIcon")
            icon.setIcon(0xe608);
            icon.setColor(0xff666666);
            icon.setAlign(1, 1);
            let div = new Div();
            div.setIcon(icon);
            div.setId("menuCheck")
            div.setRect(rect);
            div.onMouseDown(switchCheck);
            return div;
        }
        let getTextDiv = () => {
            let rect = Rect.newXYWH(650, 500, 96, 18);
            rect.setColor(0x00000000);
            let text = new Text();
            text.setText("创建开始菜单");
            text.setColor(0xff666666);
            let div = new Div();
            div.setText(text);
            div.setRect(rect);
            div.setId("menuText")
            div.onMouseDown(switchCheck);
            return div;
        }
        return [getCheckDiv(), getTextDiv()]
    }
    win.addElement([getInput(),getSelectBtn(),...getDesktopBtn(),...getMenuBtn()]);
}
let removeCustomizeElements = () => {
    win.removeElement("input");
    win.removeElement("selectBtn");
    win.removeElement("desktopCheck");
    win.removeElement("desktopText");
    win.removeElement("menuCheck");
    win.removeElement("menuText");
}
app.ready(() => {
    win = new Win(windowTitle, 800, 600); //默认为透明窗口
    initBannerArr();
    setWinDragPath();
    win.addElement([getBgRect(), getTitleDiv(), getCloseDiv(), getMinimizeDiv(),
        ...bannerArr, getStartBtn(), ...getLicenceDiv(), ...getCustomizeBtn()
    ]);
    win.show();
    setBannerAnimation();
    globalThis.win = win;
})

