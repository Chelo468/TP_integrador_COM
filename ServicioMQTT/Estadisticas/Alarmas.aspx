<%@ Page Title="Alarmas" Language="C#" MasterPageFile="~/Site.Master" AutoEventWireup="true" CodeBehind="Alarmas.aspx.cs" Inherits="Estadisticas.Alarmas" %>
<asp:Content ID="Content1" ContentPlaceHolderID="MainContent" runat="server">
    <h2>Alarmas</h2>
    <div class="row">
        <div class="col-3">
            <asp:Label ID="lblFechaDesde" runat="server" AssociatedControlID="txtFechaDesde">Fecha Desde</asp:Label>
            <asp:TextBox ID="txtFechaDesde" runat="server" CssClass="form-control" data-behavior="date"  />
        </div>
         <div class="col-3">
            <asp:Label ID="lblFechaHasta" runat="server" AssociatedControlID="txtFechaHasta">Fecha Hasta</asp:Label>
            <asp:TextBox ID="txtFechaHasta" runat="server" CssClass="form-control" data-behavior="date"  />
        </div>
        <div class="col-4">
            <asp:Label ID="lblTipo" runat="server" AssociatedControlID="ddlTipoAlarma">Tipo de Alarma</asp:Label>
            <asp:DropDownList ID="ddlTipoAlarma" runat="server" CssClass="form-select">
                <asp:ListItem Value="todas">Todas</asp:ListItem>
                <asp:ListItem Value="temperatura">Temperatura</asp:ListItem>
                <asp:ListItem Value="proximidad">Proximidad</asp:ListItem>
                <asp:ListItem Value="sonido">Sonido</asp:ListItem>
            </asp:DropDownList>
        </div>
    </div>
    <div class="row m-2">
        <div class="col-7"></div>
        <div class="col-3">
            <asp:Button ID="btnBuscar" style="float: right" runat="server" Text="Buscar" CssClass="btn btn-primary" OnClick="btnBuscar_Click" />
        </div>
    </div>

    <asp:GridView ID="grdMensajes" OnPageIndexChanging="grdMensajes_PageIndexChanging" AllowPaging="true" PageSize="25" runat="server" ClientIDMode="Static" CssClass="table table-hover" HeaderStyle-CssClass="table-dark" AutoGenerateColumns="false">
        <Columns>
            <asp:BoundField HeaderText="Tema" DataField="topic" />
            <asp:BoundField HeaderText="Mensaje" DataField="mensaje" />
            <asp:BoundField HeaderText="Fecha" DataField="fecha" />
        </Columns>
    </asp:GridView>

    <script>

        $(document).ready(function () {
            $('#hrefAlarmas').addClass('active');
        });

        $('body').find('input[data-behavior=date]').each(function () {

            $(this).datetimepicker({
                timepicker: false,
                format: 'd/m/Y'
            });

        });

    </script>

</asp:Content>
