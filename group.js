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
