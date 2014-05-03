<!DOCTYPE html>
<html>
<head>
	<script>
	var eventsJSON = JSON.parse('[["0","letsPoker"],["0","roundStarted 1"],["0","rmDealerButton 10"],["0","addDealerButton 18"],["0","dealing"],["20","receiveCard TEN of CLUBS"],["20","receiveCard SIX of HEARTS"],["0","preflop"],["0","betRound"],["40","fold"],["42","call"],["10","call"],["18","fold"],["28","call"],["20","getHandRank"],["20","getBigBlindAtRound 1"],["20","canRaise 100"],["20","getBigBlindAtRound 1"],["20","raise 100"],["42","call"],["10","call"],["28","call"],["0","burn KING of CLUBS"],["0","flop DEUCE of CLUBS,SEVEN of CLUBS,NINE of SPADES"],["0","betRound"],["28","raise 100"],["20","getHandRank"],["20","getBigBlindAtRound 1"],["20","canRaise 100"],["20","getBigBlindAtRound 1"],["20","raise 100"],["42","raise 100"],["10","call"],["28","raise 100"],["20","getHandRank"],["20","getBigBlindAtRound 1"],["20","canRaise 100"],["20","getBigBlindAtRound 1"],["20","raise 100"],["42","raise 100"],["10","fold"],["28","raise 100"],["20","getHandRank"],["20","getBigBlindAtRound 1"],["20","canRaise 100"],["20","canCall"],["20","call"],["42","call"],["0","burn JACK of DIAMONDS"],["0","turn ACE of HEARTS"],["0","betRound"],["28","check"],["20","getHandRank"],["20","getBigBlindAtRound 1"],["20","canRaise 100"],["20","canCall"],["20","canCheck"],["20","check"],["42","check"],["0","burn SEVEN of DIAMONDS"],["0","river FOUR of HEARTS"],["0","betRound"],["28","check"],["20","getHandRank"],["20","canCheck"],["20","check"],["42","check"],["0","showdown"],["28","revealCards NINE of CLUBS,TEN of SPADES"],["20","revealCards TEN of CLUBS,SIX of HEARTS"],["42","revealCards FIVE of CLUBS,QUEEN of HEARTS"],["0","roundWinners 1,Schar"],["0","handOutPot 1,Schar"],["0","collectCards"],["0","roundEnded"],["0","roundStarted 2"],["0","rmDealerButton 18"],["0","addDealerButton 28"],["0","dealing"],["20","receiveCard FOUR of HEARTS"],["20","receiveCard NINE of DIAMONDS"],["0","preflop"],["0","betRound"],["42","fold"],["10","call"],["18","fold"],["28","call"],["20","getHandRank"],["20","canCheck"],["20","getCallAmount"],["20","fold"],["40","raise 100"],["10","call"],["28","call"],["0","burn ACE of HEARTS"],["0","flop TEN of CLUBS,KING of DIAMONDS,NINE of CLUBS"],["0","betRound"],["40","raise 100"],["10","call"],["28","raise 100"],["40","raise 100"],["10","fold"],["28","raise 100"],["40","raise 100"],["28","raise 100"],["40","raise 100"],["28","raise 100"],["40","allin"],["0","burn FIVE of DIAMONDS"],["0","turn KING of CLUBS"],["0","betRound"],["40","allin"],["0","handOutPot 1,Schar"],["0","collectCards"],["0","rebuyOrLeave 20"],["20","canRebuy 1"],["0","rebuyOrLeave 40"],["0","leave 20"],["0","leave 40"],["0","roundEnded"],["0","roundStarted 3"],["0","rmDealerButton 28"],["0","addDealerButton 42"],["0","dealing"],["0","preflop"],["0","betRound"],["28","raise 100"],["42","fold"],["10","fold"],["18","fold"],["0","handOutPot 1,Schar"],["0","collectCards"],["0","rebuyOrLeave 20"],["20","canRebuy 1"],["0","rebuyOrLeave 40"],["0","leave 20"],["0","leave 40"],["0","roundEnded"],["0","roundStarted 4"],["0","rmDealerButton 42"],["0","addDealerButton 10"],["0","dealing"],["0","preflop"],["0","betRound"],["42","fold"],["10","fold"],["18","fold"],["0","handOutPot 1,Schar"],["0","collectCards"],["0","rebuyOrLeave 20"],["20","canRebuy 1"],["0","rebuyOrLeave 40"],["0","leave 20"],["0","leave 40"],["0","roundEnded"],["0","roundStarted 5"],["0","rmDealerButton 10"],["0","addDealerButton 18"],["0","dealing"],["0","preflop"],["0","betRound"],["10","fold"],["18","fold"],["28","call"],["42","fold"],["0","handOutPot 1,Schar"],["0","collectCards"],["0","rebuyOrLeave 20"],["20","canRebuy 1"],["0","rebuyOrLeave 40"],["0","rebuyOrLeave 42"],["0","leave 20"],["0","leave 40"],["0","leave 42"],["0","roundEnded"],["0","roundStarted 6"],["0","rmDealerButton 18"],["0","addDealerButton 28"],["0","dealing"],["0","preflop"],["0","betRound"],["28","call"],["10","fold"],["18","fold"],["0","handOutPot 1,Schar"],["0","collectCards"],["0","rebuyOrLeave 10"],["0","rebuyOrLeave 20"],["20","canRebuy 1"],["0","rebuyOrLeave 40"],["0","rebuyOrLeave 42"],["0","leave 10"],["0","leave 20"],["0","leave 40"],["0","leave 42"],["0","roundEnded"],["0","roundStarted 7"],["0","rmDealerButton 28"],["0","addDealerButton 18"],["0","dealing"],["0","preflop"],["0","betRound"],["28","call"],["18","allin"],["0","burn EIGHT of HEARTS"],["0","flop KING of DIAMONDS,SEVEN of CLUBS,ACE of DIAMONDS"],["0","betRound"],["28","raise 100"],["18","check"],["0","burn EIGHT of DIAMONDS"],["0","turn JACK of CLUBS"],["0","betRound"],["28","raise 100"],["18","allin"],["0","handOutPot 1,Schar"],["0","collectCards"],["0","rebuyOrLeave 10"],["0","rebuyOrLeave 18"],["0","rebuyOrLeave 20"],["20","canRebuy 1"],["0","rebuyOrLeave 40"],["0","rebuyOrLeave 42"],["0","leave 10"],["0","leave 18"],["0","leave 20"],["0","leave 40"],["0","leave 42"],["0","roundEnded"],["0","gameWinner 28"]]');
    var myBotID = 20;
	var botsJSON = JSON.parse('{"20":"Jonas3","40":"AllInBot","42":"CsikiBot","10":"bot 10","18":"bot 18","28":"MyBot11"}');
	</script>
    <script src="scripts/createjs-2013.12.12.min.js"></script>
    <script>
    	var suits = ["HEARTS", "DIAMONDS", "CLUBS", "SPADES", "NULL"];
		var ranks = ["DEUCE of ", "THREE of ", "FOUR of ", "FIVE of ", "SIX of ", "SEVEN of ", "EIGHT of ",
    					"NINE of ", "TEN of ", "JACK of ", "QUEEN of ", "KING of ", "ACE of ", "NULL of "];
    
    	var stage;
    	var loadqueue;

    	var loadComleted = false;
    	
    	var loadingCircle;
    	var loadingCircleGoesLeft = false;
    	var loadingText;
    	
    	var selfPosition = [260, 342];
    	var playerPositions = [[79, 328], [44, 142], [146, 60], [490, 141], [440, 330]];
    	var cardPositions = [[110, 260], [91, 177], [140, 126], [407, 177], [390, 260]];

    	var dealerButtonOwner;
    	
    	var bots = {};

    	function Bot(botID)
    	{
    		this.botID = botID;
    		this.active = true;
    		this.cards = [];
    		this.name = "Bot";
    		this.chips = 2000;
    		this.nameText = null;
    		this.chipsText = null;
    	}

        function init() {
            stage = new createjs.Stage("demoCanvas");
            
            createjs.Ticker.addEventListener("tick", tick);
            createjs.Ticker.setFPS(40);
            
            loadqueue = new createjs.LoadQueue(false);
            loadqueue.on("complete", downLoadFinished, this);
            loadqueue.loadManifest([
     			{id: "table", src:"images/game/table.png"}
 			]);

            loadingText = new createjs.Text("Please wait", "20px Arial", "#000000");
            loadingText.x = stage.canvas.width / 2 - loadingText.getBounds().width/2;
			loadingText.y = stage.canvas.height / 2 - 50;
            loadingText.textBaseline = "top";
			stage.addChild(loadingText);

			loadingCircle = new createjs.Shape();
            loadingCircle.graphics.beginFill("blue").drawCircle(0,0,5);
			loadingCircle.x = 200;
			loadingCircle.y = stage.canvas.height / 2;
			stage.addChild(loadingCircle);
       		
       		stage.on("stagemouseup", function(evt) {
			    console.log("stageX/Y: "+evt.stageX+","+evt.stageY); // always in bounds
			    console.log("rawX/Y: "+evt.rawX+","+evt.rawY); // could be < 0, or > width/height
			});
        }
        
        function tick(event)
        {
        	if(!loadComleted)
        	{
	        	if (loadingCircleGoesLeft && loadingCircle.x < 200) {  loadingCircleGoesLeft = !loadingCircleGoesLeft; }
	        	else if (!loadingCircleGoesLeft && loadingCircle.x > stage.canvas.width - 200) {  loadingCircleGoesLeft = !loadingCircleGoesLeft; }
	        	loadingCircle.x += event.delta/2 * (loadingCircleGoesLeft? -1 : 1);
       		}
       		
        	stage.update();
        }
        
        function downLoadFinished()
        {
        	loadComleted = true;
			stage.removeChild(loadingCircle);
			stage.removeChild(loadingText);
			
			var bmp = new createjs.Bitmap(loadqueue.getResult("table"));
			bmp.cache(0, 0, 410, 221);
			
            bmp.x = stage.canvas.width / 2 - bmp.getBounds().width/2;
			bmp.y = 100;
		 	stage.addChild(bmp);
		 	parseNextEvent(["26", "letsPoker"]);
			
			for(var i=0; i<5; i++)
			{
				var c = new createjs.Shape();
	            c.graphics.beginFill("red").drawCircle(0,0,5);
				c.x = cardPositions[i][0];
				c.y = cardPositions[i][1];
				stage.addChild(c);
			}
        }
        
        function letsPoker()
        {
        	var i = 0;
        	for(var id in botsJSON)
        	{
        		var bot = new Bot(id);
        		bot.name = botsJSON[id];
            	bot.nameText = new createjs.Text(bot.name, "14px Arial", "#000000");
            	if (bot.botID == myBotID)
            	{
            		bot.nameText.x = selfPosition[0] - bot.nameText.getBounds().width / 2;
					bot.nameText.y = selfPosition[1];
            	}
            	else
            	{
            		bot.nameText.x = playerPositions[i][0] - bot.nameText.getBounds().width / 2;
					bot.nameText.y = playerPositions[i][1];
        			i++;
				}
            	bot.nameText.textBaseline = "top";
            	bot.nameText.shadow = new createjs.Shadow("#ffffff", 0, 0, 5);
				stage.addChild(bot.nameText);
				
				bot.chipsText = new createjs.Text(bot.chips, "bold 18px Arial", "#CC0000");
        		bot.chipsText.x = bot.nameText.x;
				bot.chipsText.y = bot.nameText.y + 16;
            	bot.chipsText.textBaseline = "top";
            	bot.chipsText.shadow = new createjs.Shadow("#ffffff", 0, 0, 5);
				stage.addChild(bot.chipsText);
				
        		bots[id] = bot;
        	}
        }
        
        function roundStarted(roundNum)
        {
        	
        }
        
        function dealing()
        {
        
        }
        
        function receiveCard()
        {
        
        }
                
        function rmDealerButton(botID)
        {
        	dealerButtonOwner = null;
        }
        
        function addDealerButton(botID)
        {
        	dealerButtonOwner = bots[botID];
        }
        
        function parseNextEvent(event)
        {
        	var logger = event[0];
        	var msg = event[1].split(" ");
        	if(logger == "0")
		 		executeFunctionByName(msg[0], window, msg.splice(1));
		 	else
		 		executeFunctionByName(msg[0], window, msg.splice(1).splice(0, 0, logger));
        }
        
        function executeFunctionByName(functionName, context, argsArray) {
			//var argsArray = [].slice.call(arguments).splice(2);
			var namespaces = functionName.split(".");
			var func = namespaces.pop();
			for(var i = 0; i < namespaces.length; i++) {
				context = context[namespaces[i]];
			}
			return context[func].apply(this, argsArray);
		}
        
    </script>
</head>
<body onLoad="init();">
    <canvas id="demoCanvas" width="530" height="500" style="background-color:#DEFFFF">
    	<?= $tr["GAME_BROWSER_INCOMPATIBLE"] ?>
    </canvas>
</body>
</html>