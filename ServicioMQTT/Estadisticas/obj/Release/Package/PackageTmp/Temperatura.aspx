<%@ Page Title="Temperaturas" Language="C#" MasterPageFile="~/Site.Master" AutoEventWireup="true" CodeBehind="Temperatura.aspx.cs" Inherits="Estadisticas.Temperatura" %>
<asp:Content ID="Content1" ContentPlaceHolderID="MainContent" runat="server">

    <style>
        .oculto{
            display: none;
        }
    </style>

    <h2>Temperaturas</h2>
    <div class="row">
        <div class="col-2">
            <asp:Label ID="lblFechaDesde" runat="server" AssociatedControlID="txtFechaDesde">Fecha Desde</asp:Label>
            <asp:TextBox ID="txtFechaDesde" runat="server" CssClass="form-control" data-behavior="date" />
        </div>
         <div class="col-2">
            <asp:Label ID="lblFechaHasta" runat="server" AssociatedControlID="txtFechaHasta">Fecha Hasta</asp:Label>
            <asp:TextBox ID="txtFechaHasta" runat="server" CssClass="form-control" data-behavior="date" />
        </div>
        
    </div>
    <div class="row m-2">
        <div class="col-1"></div>
        <div class="col-3">
            <asp:Button ID="btnBuscar" style="float: right" runat="server" Text="Buscar" CssClass="btn btn-primary" OnClick="btnBuscar_Click" />
        </div>
    </div>

    <div class="row oculto">
        <asp:GridView ID="grdMensajes" runat="server" ClientIDMode="Static" CssClass="table table-hover" HeaderStyle-CssClass="oculto">

        </asp:GridView>
    </div>

    <div id="chartUnico" class="col-12">
    </div>

    <canvas id="myChart" width="400" height="400"></canvas>
    <script>

        $(document).ready(function () {
            $('#hrefTemperatura').addClass('active');
        });

        $('body').find('input[data-behavior=date]').each(function () {

            $(this).datetimepicker({
                timepicker: false,
                format: 'd/m/Y'
            });

        });

        function doDataTable() {

            var chartUnico = document.getElementById('chartUnico');

            var tableGrdMensajes = $('#grdMensajes').DataTable({
                'sDom': 'rtlfip',
                'bFilter': false,
                'paging': false,
                'bInfo': false,  
                'columns': [
                    { "data": "id_mensaje", "sTitle": "Id Mensaje", "bSortable": true, "bVisible": true },
                    { "data": "topic", "sTitle": "Tema", "bSortable": true, "bVisible": true },
                    { "data": "mensaje", "sTitle": "Mensaje", "bSortable": true, "bVisible": true },
                    { "data": "fecha", "sTitle": "Fecha", "bSortable": true, "bVisible": true },
                ],
                'aaSorting': [], 
                'oLanguage': {
                    'oPaginate': {
                        'sNext': 'Siguiente',
                        'sPrevious': 'Anterior'
                    },
                    'sLengthMenu': 'Mostar _MENU_ registros',
                    'sInfo': 'Registros del _START_ al _END_  -  Total: _TOTAL_',
                    'sSearch': 'Buscar:',
                    'sEmptyTable': 'No hay registros en la tabla',
                    'sInfoEmpty': 'No hay registros filtrados',
                    'sInfoFiltered': ' de un total de _MAX_ ',
                    'sZeroRecords': 'No hay registros para mostrar'
                },
                'bDestroy': true
            });

            //Array que va a tener las temperaturas
            var temperaturas = new Array();

            //Array que guarda las horas de cada temperatura
            var horas = new Array();

            var contador = 1;

            var objeto = {};

            objeto.temperatura = "20";

            tableGrdMensajes.rows().every(function () {

                if (Number(this.data()["mensaje"])) {
                    temperaturas.push(Number(this.data()["mensaje"]));

                    horas.push(this.data()["fecha"].substr(this.data()["fecha"].indexOf(' ')));
                    
                }
            });

            

            var divCanvas = document.createElement("div");

            var canvas = document.createElement("canvas");

            canvas.id = "chart";
            canvas.className = "grafico-canvas";
            canvas.style.cursor = 'pointer';

            //canvas.onclick = function (e) { console.log(e); mostrarDatos(this) };

            var temperaturasData = {
                label: 'Temperaturas',
                backgroundColor: 'rgba(0, 99, 132, 0.6)',
                borderColor: 'rgba(0, 99, 132, 1)',
                data: temperaturas//emitidas
            };

            var myChart = new Chart(canvas, {
                type: 'bar',
                data: {
                    labels: horas,
                    datasets: [temperaturasData]
                }
            });

            chartUnico.appendChild(canvas);

        }

    </script>
</asp:Content>
