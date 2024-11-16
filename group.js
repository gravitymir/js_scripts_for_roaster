    function runBar() {
        document.getElementById("main").style.marginRight = "-3000px";
        let kitchen_or_bar = "bar";
        let wds = document.getElementById(`calendar_${kitchen_or_bar}`);
        wds.childNodes[1].classList.add("hide");//input
        let wdsResult = wds.childNodes[1].value.split('-W');

        wds.childNodes[3].innerHTML = `${wdsResult[0]}&nbsp;Week&nbsp;<b>${wdsResult[1]}</b>`;
        wds.childNodes[3].classList.remove("hide");//span with class hide

        let bar = document.getElementById(`conteiner_${kitchen_or_bar}`);
        let colums = bar.getElementsByClassName("column");

        for (let c = 1; c < colums.length; c++) {
            let selects = colums[c].getElementsByTagName("select");
            let check = true;
            for (let s = 0; s < selects.length; s++) {
                if (selects[s].value != "OFF") {
                    check = false;
                    break;
                }
            }
            if (check) {
                colums[c].style.display = "none";
            }
        }

        let selects = bar.getElementsByTagName("select");
        for (let s = 0; s < selects.length; s++) {
            selects[s].style.display = "none";
            let p = document.createElement('p');
            p.innerHTML = selects[s].value;
            p.classList = `p${selects[s].value}`;
            p.style.fontSize = "20px";
            selects[s].parentNode.appendChild(p);
        }

        let messageSTR = countFromMatrix(kitchen_or_bar);
        sendImageToTelegram({
            wdsResult: wdsResult,
            caption: `<code>${messageSTR}</code>#roster${kitchen_or_bar}pcb\n${wdsResult[0]} week <b>${wdsResult[1]}</b>\n\n<code>${MATRIX_DATA[kitchen_or_bar].join("")}</code>`,
            chat_id: GROUP,
            conteiner: kitchen_or_bar,
            show_in_terminal: true,
            disable_notification: true,
            has_spoiler: false
        });
        document.getElementById("main").style.display = "none";
        document.getElementById("main").style.marginLeft = "-3000px";

        setTimeout(_ => {
            // for (let s = 0; s < selects.length; s++) {
            //     selects[s].style.display = "";
            //     selects[s].parentNode.getElementsByTagName("p")[0].remove();
            // }
            // for (let c = 1; c < colums.length; c++) {
            //     colums[c].style.display = "flex";
            // }
            // document.getElementById("main").style.marginRight = "0";
            // document.getElementById("main").style.display = "";
            clearTerminal();

            // wds.childNodes[1].classList.remove("hide");
            // wds.childNodes[3].innerHTML = "";//span with class hide
            // wds.childNodes[3].classList.add("hide");//span with class hide
        }, 5000);
    }

function runKitchen() {
        document.getElementById("main").style.marginRight = "-3000px";
        let kitchen_or_bar = "kitchen";
        let wds = document.getElementById(`calendar_${kitchen_or_bar}`);
        wds.childNodes[1].classList.add("hide");//input
        let wdsResult = wds.childNodes[1].value.split('-W');

        wds.childNodes[3].innerHTML = `${wdsResult[0]}&nbsp;Week&nbsp;<b>${wdsResult[1]}</b>`;
        wds.childNodes[3].classList.remove("hide");//span with class hide

        // let convertObj = {
        //     "H": "H",
        //     "O": "O",
        //     "L": "L",
        //     "I": "I",
        //     "D": "D",
        //     "A": "A",
        //     "Y": "Y",
        //     "OFF": "OFF",
        //     "9-5": "9am-5pm",
        //     "4-0": "4pm-12pm",
        //     "Day": "Day",
        //     "11-9": "11am-9pm",
        //     "12-X": "12pm-10pm",
        //     "2-X": "2pm-10pm",
        //     "2-10": "2pm-10p",
        //     //
        //     "9am-5pm": "9-5",
        //     "4pm-12pm": "4-0",
        //     "Day": "Day",
        //     "11am-9pm": "11-9",
        //     "12pm-10pm": "12-X",
        //     "2pm-10pm": "2-X",
        //     "2pm-10p": "2-10"
        // };

        let conteiner = document.getElementById(`conteiner_${kitchen_or_bar}`);
        let selects = conteiner.getElementsByTagName("select");
        for (let i = 0; i < selects.length; i++) {
            selects[i].classList.add("hide");
            let p = document.createElement('p');
            p.innerHTML = selects[i].value;
            //p.style.fontSize = "9px";
            selects[i].parentNode.appendChild(p);
        }

        checkRowsOnHoliday(kitchen_or_bar);
        sendImageToTelegram({
            wdsResult: wdsResult,
            caption: "",
            chat_id: GROUP,
            conteiner: `${kitchen_or_bar}`,
            show_in_terminal: true,
            disable_notification: true,
            has_spoiler: true
        });//to gearoid


        for (let i = 0; i < selects.length; i++) {
            let p = selects[i].parentElement.getElementsByTagName("p")[0];
            p.innerHTML = p.innerHTML;
            p.style.fontSize = "20px";
            p.classList = `p${p.innerHTML}`;
        };

        let messageSTR = countFromMatrix(kitchen_or_bar);

        setTimeout(() => {
            sendImageToTelegram({
                wdsResult: wdsResult,
                chat_id: GROUP,
                caption: `<code>${messageSTR}</code>#roster${kitchen_or_bar}pcb\n${wdsResult[0]} week <b>${wdsResult[1]}</b>\n\n<code>${MATRIX_DATA[kitchen_or_bar].join("")}</code>`,
                conteiner: kitchen_or_bar,
                show_in_terminal: true,
                disable_notification: true,
                has_spoiler: false
            });
            document.getElementById("main").style.display = "none";
            document.getElementById("main").style.marginRight = "-3000";
        }, 2000, false);
    }
(function init(){
        document.getElementById("main").style.marginLeft = "0px";
        document.getElementById("main").style.display = "flex";
        let selects = document.getElementsByTagName("select");
        for (let i = 0; i < selects.length; i++) {
            selects[i].disabled = false;
        }
        fillDate("bar");
        fillDate("kitchen");
    })()
