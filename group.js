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
