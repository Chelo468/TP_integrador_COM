<%@ Page Title="Consola" Language="C#" MasterPageFile="~/Site.Master" AutoEventWireup="true" CodeBehind="Consola.aspx.cs" Inherits="Estadisticas.Consola" %>
<asp:Content ID="Content1" ContentPlaceHolderID="MainContent" runat="server">

    <h2>Consola</h2>
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

    <asp:GridView ID="grdMensajes" runat="server" ClientIDMode="Static" CssClass="table table-hover" HeaderStyle-CssClass="table-dark" AutoGenerateColumns="false">
        <Columns>
            <asp:BoundField HeaderText="Tema" DataField="topic" />
            <asp:BoundField HeaderText="Mensaje" DataField="mensaje" />
            <asp:BoundField HeaderText="Fecha" DataField="fecha" />
        </Columns>
    </asp:GridView>

    <script>

        $(document).ready(function () {
            $('#hrefConsola').addClass('active');
        });

        $('body').find('input[data-behavior=date]').each(function () {

            $(this).datetimepicker({
                timepicker: false,
                format: 'd/m/Y'
            });

        });

    </script>

</asp:Content>
