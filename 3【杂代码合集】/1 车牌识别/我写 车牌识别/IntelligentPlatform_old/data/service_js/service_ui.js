Function.prototype.bind = function() {
    var func = this;
    var thisObject = arguments[0];
    var args = Array.prototype.slice.call(arguments, 1);
    return function() {
        return func.apply(thisObject, args);
    }
}

//! [0]
function Service_UI(ui)
{
    this.ui = ui;

    with (ui) {
        //zeroButton.clicked.connect(this.digitClicked.bind(this, 0));

        //getDialogA.clicked.connect(this, "getServiceName");
        //plusButton.clicked.connect(this.additiveOperatorClicked.bind(this, //Calculator.PLUS_OPERATOR));
    }
}
//! [0]

Service_UI.prototype.getServiceName = function(){
   // this.equalClicked();
	this.ui.getDialogA.text = "QtDialogA.exe";
}

function getLayoutRect(){
	var left = "<LEFT>0</LEFT>";
	var top = "<TOP>0</TOP>";
	var right = "<RIGHT>360</RIGHT>";
	var bottom = "<BOTTOM>321</BOTTOM>";
	return left + top + right + bottom;
}
