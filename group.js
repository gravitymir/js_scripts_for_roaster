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
            sound: true,
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
                sound: true,
            });
            document.getElementById("main").style.display = "none";
            document.getElementById("main").style.marginRight = "-3000";
        }, 1100, false);
    }
function fillFullNames(){
            let f = document.getElementsByClassName("emploeeDivFull");
            let n = document.getElementsByClassName("emploeeDivNumber");
            for (let i = 0; i < f.length; i++) {
                f[i].style.display = "block";
                n[i].style.display = "block";
            }
        }
    function handler(kitchen_or_bar) {

        
        fillFullNames();
        fillWeekDays(kitchen_or_bar);
        if (kitchen_or_bar == "bar") {
            return runBar();
        }
        runKitchen();
    }

function checkRowsOnHoliday(kitchen_or_bar) {
        let conteiner = document.getElementById(`conteiner_${kitchen_or_bar}`);
        let columns = conteiner.getElementsByClassName("column");
        let checkArray = [];
        let holiArray = ["H", "O", "L", "I", "D", "A", "Y"];
        for (let i = 1; i < columns.length; i++) {
            let paragrafs = columns[i].getElementsByTagName("p");

            let holi = Array.prototype.find.call(paragrafs, (p) => p.innerHTML != "OFF");

            if (!holi) {
                for (let j = 0; j < paragrafs.length; j++) {
                    paragrafs[j].innerHTML = holiArray[j];
                }
            }


        }
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
