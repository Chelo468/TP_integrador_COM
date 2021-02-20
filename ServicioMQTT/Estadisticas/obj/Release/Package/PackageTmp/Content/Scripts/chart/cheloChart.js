function Chart2(div, pTitulo, objetos, referencias, idDiv) {
    //Cada chart es un gráfico nuevo
    //titulo = Titulo del grafico
    //objetos [{filtro: filtro, emitidas: cantEmitidas, revocadas: cantRevocadas}]
    
    var divContenedorInterno = document.createElement("div");

    console.log("Objetos: ", objetos);

    divContenedorInterno.id = idDiv
    divContenedorInterno.className = "graphic-frame";

    //divContenedorInterno.style.height = "250px"
    //divContenedorInterno.style.marginTop = "15px"
    //divContenedorInterno.style.textAlign = "center"
    //divContenedorInterno.style.paddingTop = "10px"
    

    var tituloSpan = document.createElement("span")
    tituloSpan.className = "graphic-title";
    tituloSpan.innerHTML = pTitulo;

    var divContenedorDeGraficos = document.createElement("div")
    divContenedorDeGraficos.className = "graphic-content";

    var alturaGrafico = 100 / objetos.length

    var mayorValor = 0;

    for (var i = 0; i < objetos.length; i++) {
        if(i == 0)
        {
            mayorValor = objetos[i];
        }
        else {
            if(objetos[i] > mayorValor)
            {
                mayorValor = objetos[i]
            }

        }
    }

    for (var i = 0; i < objetos.length; i++) {
        
        var divEtiquetaGrafico = document.createElement("div")
        divEtiquetaGrafico.className = "graphic-label";
        divEtiquetaGrafico.style.height = alturaGrafico + "%"

        var spanEtiqueta = document.createElement("span")
        spanEtiqueta.innerHTML = objetos[i].filtro;
        

        divEtiquetaGrafico.appendChild(spanEtiqueta)

        var divGrafico = document.createElement("div");
        divGrafico.className = "graphic";
        divGrafico.style.height = alturaGrafico + "%";
        divGrafico.style.marginTop = "1em";

        if(i > 0)
            divGrafico.style.borderTop = "1px solid #ccc";
        
        var divEmitidasGraf = document.createElement("div")
        divEmitidasGraf.className = "graphic-emitidas";

        var divPintadoEmitidasGraf = document.createElement("div")
        divPintadoEmitidasGraf.className = "graphic-emitidas-painted";


        divPintadoEmitidasGraf.style.width = (objetos[i] / mayorValor) * 100 + "%"
        divPintadoEmitidasGraf.innerHTML = objetos[i]

        if (objetos[i] == 0)
            divPintadoEmitidasGraf.style.color = "white"

        divEmitidasGraf.appendChild(divPintadoEmitidasGraf)

        //var divRevocadasGraf = document.createElement("div")
        //divRevocadasGraf.className = "graphic-revocadas";

        //var divPintadoRevocadasGraf = document.createElement("div")
        //divPintadoRevocadasGraf.className = "graphic-revocadas-painted";

        //divPintadoRevocadasGraf.style.width = (objetos[i] / mayorValor) * 100 + "%"
        //divPintadoRevocadasGraf.innerHTML = objetos[i]

        //if (objetos[i].revocados == 0)
        //    divPintadoRevocadasGraf.style.color = "white"

        //divRevocadasGraf.appendChild(divPintadoRevocadasGraf)

        var divReferencias = document.createElement("div")
        divReferencias.className = "references-group";

        var spanEmitidas = document.createElement("span")
        spanEmitidas.className = "count-emitidas-reference";
        
        spanEmitidas.innerHTML = objetos[i];

        //var spanRevocadas = document.createElement("span")
        //spanRevocadas.className = "count-revocadas-reference";
        
        //spanRevocadas.innerHTML = objetos[i].revocados;

        var divReferenciaColorEm = document.createElement("div")
        divReferenciaColorEm.className = "emitidas-color-reference";
        

        //var divReferenciaColorRev = document.createElement("div")
        //divReferenciaColorRev.className = "revocadas-color-reference";
        

        var spanNombreEmitidas = document.createElement("span")
        spanNombreEmitidas.className = "emitidas-reference";
        
        spanNombreEmitidas.innerHTML = "Temperaturas"

        //var spanNombreRevocadas = document.createElement("span")
        //spanNombreRevocadas.className = "revocadas-reference";
        
        //spanNombreRevocadas.innerHTML = "Revocados"
        

        divReferencias.appendChild(spanEmitidas)
        divReferencias.appendChild(divReferenciaColorEm)
        divReferencias.appendChild(spanNombreEmitidas)

        //divReferencias.appendChild(spanRevocadas)
        //divReferencias.appendChild(divReferenciaColorRev)
        //divReferencias.appendChild(spanNombreRevocadas)

        if (mayorValor > 0) {
            divGrafico.appendChild(divEmitidasGraf)
            //divGrafico.appendChild(divRevocadasGraf)
        }
        
        divGrafico.appendChild(divReferencias)
        


        var divClearFix = document.createElement("div")
        divClearFix.className = "clearfix"

        divContenedorDeGraficos.appendChild(divEtiquetaGrafico);
        divContenedorDeGraficos.appendChild(divGrafico);
        divContenedorDeGraficos.appendChild(divClearFix);

        //console.log(objetos[i].filtro)
    }

 

    var divSpanTitle = document.createElement("div")
    divSpanTitle.className = "div-graphic-title";
    //divSpanTitle.style.display = "table-header-group"

    let internDiv = idDiv

    divSpanTitle.appendChild(tituloSpan);
    divContenedorInterno.appendChild(divSpanTitle)
    divContenedorInterno.appendChild(divContenedorDeGraficos);
    //divContenedorInterno.appendChild(img)
    

    div.appendChild(divContenedorInterno)
    
}

function grafico2Img(idDiv, titulo) {
    PrintElem(idDiv, titulo)


    //html2canvas(document.getElementById(idDiv), {
    //    onrendered: function (canvas) {
    //        // document.body.appendChild(canvas);
    //        console.log("asdasdsa")
    //        window.open(canvas.toDataURL('image/jpeg'));
    //    }
    //});

}

function PrintElem(elem, titulo) {

    var idPolitica = elem.replace("divGrafico", "");

    $.ajax({
        url: emitidasRevocadasURL.generarImagen,
        data: { idPolitica: idPolitica },
        success: function (data) {
            console.log(data);
        }
    });

    //var mywindow = window.open('', '', 'height=400,width=600');

    //mywindow.document.write('<html><head><title>' + document.title + '</title>');
    //mywindow.document.write('<link href="http://localhost:51162/Styles/chart/cheloChart.css" rel="stylesheet" />')
    //mywindow.document.write('</head><body >');
    ////mywindow.document.write('<h1>' + titulo + '</h1>');
    //mywindow.document.write(document.getElementById(elem).innerHTML);
    //mywindow.document.write('</body></html>');

    //mywindow.document.close(); // necessary for IE >= 10
    //mywindow.focus(); // necessary for IE >= 10*/

    //mywindow.print();
    ////mywindow.close();

    //return true;
}