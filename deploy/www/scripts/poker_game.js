var eventsJSONLen = 0;
var suits = ["HEARTS", "DIAMONDS", "CLUBS", "SPADES"];
var ranks = ["DEUCE", "THREE", "FOUR", "FIVE", "SIX", "SEVEN", "EIGHT",
                "NINE", "TEN", "JACK", "QUEEN", "KING", "ACE"];
var cardNames = [];

var stage;
var loadqueue;

var loadComleted = false;

var loadingCircle;
var loadingCircleGoesLeft = false;
var loadingText;

var eventText;

var running = false;
var animationRunning = false;

var cardScale = 0.3;
var cardsSpriteSheet;

var playerPositions = [[260, 342], [79, 328], [44, 142], [146, 60], [490, 141], [440, 330]];
var cardPositions = [[225, 262], [110, 262], [52, 177], [140, 100], [407, 177], [340, 262]];
var dealerPosition = [265, 130];

var cardsOnTable = [];
var cardsOnTablePosition = [170, 177];
var chipsOnTable = 0;
var chipsOnTableText;

var dealerButton;
var dealerButtonOwner;

var bots = {};

var animations = [];
var speed = 1;
var desiredSpeed = 1;

var currentEvent = -1;
var targetEvent = -1;
var lastAddedObjectID = 0;

var playButton, pauseButton;

function Bot(botID) {
    this.botID = botID;
    this.cards = [];
    this.name = "Bot";
    this.chips = 2000;
    this.nameText = null;
    this.chipsText = null;
    this.seat = 0;
    this.hasLeft = false;
    this.getChips = function () {
        return this.chips;
    }
    this.setChips = function (amount) {
        this.chips = amount;
        this.chipsText.text = amount;
    }
}

function init() {
    stage = new createjs.Stage("gameCanvas");

    createjs.Ticker.addEventListener("tick", tick);
    createjs.Ticker.setFPS(40);

    loadqueue = new createjs.LoadQueue(false);
    loadqueue.on("complete", downLoadFinished, this);
    loadqueue.loadManifest([
        { id: "table", src: "images/game/table.png" },
        { id: "cards", src: "images/cards.png" },
        { id: "dealer", src: "images/game/dealer_button.png" }
    ]);

    loadingText = new createjs.Text("Please wait", "20px Arial", "#ffffff");
    loadingText.x = stage.canvas.width / 2 - loadingText.getBounds().width / 2;
    loadingText.y = stage.canvas.height / 2 - 50;
    loadingText.textBaseline = "top";
    stage.addChild(loadingText);

    loadingCircle = new createjs.Shape();
    loadingCircle.graphics.beginFill("blue").drawCircle(0, 0, 5);
    loadingCircle.x = 200;
    loadingCircle.y = stage.canvas.height / 2;
    stage.addChild(loadingCircle);

    for (var s = 0; s < suits.length; ++s) {
        for (r = 0; r < ranks.length; ++r) {
            cname = suits[s] + " " + ranks[r];
            cardNames.push(cname);
        }
    }
    cardNames.push('NULL NULL');

    stage.on("stagemouseup", function (evt) {
        console.log("stageX/Y: " + evt.stageX + "," + evt.stageY); // always in bounds
    });

    for (var k in eventsJSON) {
        eventsJSONLen++;
    }

    playButton = document.getElementById("playButton");
    pauseButton = document.getElementById("pauseButton");
}

function tick(event) {
    if (!loadComleted) {
        if (loadingCircleGoesLeft && loadingCircle.x < 200) { loadingCircleGoesLeft = !loadingCircleGoesLeft; }
        else if (!loadingCircleGoesLeft && loadingCircle.x > stage.canvas.width - 200) { loadingCircleGoesLeft = !loadingCircleGoesLeft; }
        loadingCircle.x += event.delta / 2 * (loadingCircleGoesLeft ? -1 : 1);
        return;
    }

    if (animations.length != 0) {
        playNextAnimation();
    }
    if (running && !animationRunning) {
        if (currentEvent != eventsJSONLen - 1) {
            currentEvent++;
            parseEvent(eventsJSON[currentEvent]);
            if (targetEvent == -1 || targetEvent == currentEvent) {
                var logRow = $("#logTable tbody tr").eq(currentEvent);
                logRow.parents("div").eq(0).scrollTop(logRow.position().top - logRow.parent().position().top);
            }
            eventText.text = "Event #" + (currentEvent + 1);
        }
        if (targetEvent != -1) {
            if (targetEvent < currentEvent) {
                resetGame();
            }
            else if (targetEvent == currentEvent) {
                speed = desiredSpeed;
                running = false;
                speed = desiredSpeed;
                targetEvent = -1;
                playButton.disabled = false;
                pauseButton.disabled = false;
            }
        }
    }
    stage.update();
}

function downLoadFinished() {
    loadComleted = true;
    stage.removeChild(loadingCircle);
    stage.removeChild(loadingText);

    var bmp = new createjs.Bitmap(loadqueue.getResult("table"));
    bmp.cache(0, 0, 410, 221);

    bmp.x = stage.canvas.width / 2 - bmp.getBounds().width / 2;
    bmp.y = 100;
    stage.addChild(bmp);

    dealerButton = new createjs.Bitmap(loadqueue.getResult("dealer"));
    dealerButton.cache(0, 0, 25, 25);
    dealerButton.x = dealerPosition[0];
    dealerButton.y = dealerPosition[1];
    dealerButton.visible = false;
    stage.addChild(dealerButton);

    eventText = new createjs.Text("", "14px Arial", "#FFFFFF");
    eventText.x = 5;
    eventText.y = 5;
    eventText.textBaseline = "top";
    stage.addChild(eventText);

    chipsOnTableText = new createjs.Text("", "16px Arial", "#FFFFFF");
    chipsOnTableText.x = dealerPosition[0] - 20;
    chipsOnTableText.y = dealerPosition[1];
    chipsOnTableText.textBaseline = "top";
    stage.addChild(chipsOnTableText);

    var data = {
        images: [loadqueue.getResult("cards")],
        frames: { width: 125, height: 181, count: 58, regX: 0, regY: 0 }
    };
    cardsSpriteSheet = new createjs.SpriteSheet(data);
    lastAddedObjectID = stage.children.length - 1;
}

function playNextAnimation() {
    animationRunning = false;
    if (animations.length != 0) {
        animationRunning = true;
        var a = animations.shift();
        a();
    }
}

function playGame() {
    running = true;
}

function pauseGame() {
    running = false;
}

function parseEvent(event) {
    var logger = event[0];
    var params = event[1].split(",");
    var functionName;
    if (params.length == 1) {
        params = event[1].split(" ");
        functionName = params.splice(0, 1)[0];
    }
    else {
        //get function name
        var a = params[0].split(" ");
        functionName = a.splice(0, 1)[0];
        //cut function name from the first element in array
        params[0] = params[0].substring(functionName.length + 1);
        params = [params];
    }
    if (logger != "0")
        params.splice(0, 0, logger)
    if (functionName.indexOf("get") != 0 && functionName.indexOf("can") != 0)
        executeFunctionByName(functionName, window, params);
}

function executeFunctionByName(functionName, context, argsArray) {
    //var argsArray = [].slice.call(arguments).splice(2);
    var namespaces = functionName.split(".");
    var func = namespaces.pop();
    for (var i = 0; i < namespaces.length; i++) {
        context = context[namespaces[i]];
    }
    if (context[func] == undefined)
        console.log(functionName);
    return context[func].apply(this, argsArray);
}

function animateMessage(message) {
    var t = new createjs.Text(message, "18px Arial", "#FFFFFF");
    t.x = stage.canvas.width / 2 - t.getBounds().width / 2;
    t.y = 10;
    t.alpha = 0;
    t.textBaseline = "top";
    stage.addChild(t);
    var f = function animate() {
        createjs.Tween.get(t)
             .to({ y: 30, alpha: 1 }, 300 * speed).wait(1000 * speed).to({ alpha: 0, visible: false }, 200 * speed)
             .call(removeText)
             .call(playNextAnimation);
        function removeText() {
            stage.removeChild(t);
        }
    }
    animations.push(f);
}

function showMessageAtBot(bot, message) {
    var t = new createjs.Text(message, "20px Arial", "#FFFFFF");

    t.x = bot.cards[1].x - t.getBounds().width / 2 - 2;
    t.y = bot.cards[1].y + 20;
    t.textBaseline = "top";

    var b = new createjs.Shape();
    b.graphics.beginFill("#5286ff").drawRoundRect(0, 0, t.getBounds().width + 4, t.getBounds().height + 4, 5);
    b.x = t.x - 2;
    b.y = t.y - 2;
    b.alpha = 0.9;
    stage.addChild(b);
    stage.addChild(t);
    var f = function animate() {
        createjs.Tween.get(b)
                .wait(1000 * speed).to({ alpha: 0, visible: false }, 200 * speed);
        createjs.Tween.get(t)
             .wait(1000 * speed).to({ alpha: 0, visible: false }, 200 * speed)
             .call(removeText)
             .call(playNextAnimation);
        function removeText() {
            stage.removeChild(t);
            stage.removeChild(b);
        }
    }
    animations.push(f);
}

function getChipsOnTable() {
    return chipsOnTable;
}

function setChipsOnTable(amount) {
    amount = parseInt(amount);
    chipsOnTable = amount;
    chipsOnTableText.text = amount == 0 ? "" : amount;
}

function resetGame() {
    eventText.text = "";

    dealerButtonOwner = null;
    dealerButton.x = dealerPosition[0];
    dealerButton.y = dealerPosition[1];
    dealerButton.visible = false;

    for (var i = stage.children.length - 1; i > lastAddedObjectID; i--)
        stage.removeChildAt(i);
    running = true;
    animationRunning = false;
    cardsOnTable = [];
    setChipsOnTable(0);
    dealerButtonOwner = null;
    bots = {};
    animations = [];
    currentEvent = -1;
}

function letsPoker() {
    for (var key in botsJSON) {
        var bot = new Bot(parseInt(botsJSON[key][0]));
        bot.name = botsJSON[key][1];
        bot.seat = parseInt(key);
        bot.nameText = new createjs.Text(bot.name, "14px Arial", "#FFFFFF");
        bot.nameText.x = playerPositions[bot.seat][0] - bot.nameText.getBounds().width / 2;
        bot.nameText.y = playerPositions[bot.seat][1];
        bot.nameText.textBaseline = "top";
        stage.addChild(bot.nameText);

        bot.chipsText = new createjs.Text(bot.chips, "bold 18px Arial", "#CC0000");
        bot.chipsText.x = bot.nameText.x;
        bot.chipsText.y = bot.nameText.y + 16;
        bot.chipsText.textBaseline = "top";
        stage.addChild(bot.chipsText);

        bots[bot.botID] = bot;
    }
}

function startingChips(amount) {
    for (var i in bots) {
        bots[i].setChips(parseInt(amount));
    }
}

function roundStarted(roundNum) {
    animateMessage("Round #" + roundNum + " started");
}

function dealing() {
    animateMessage("Dealing");
    function giveCard(bot, right) {
        return function () {
            var card = createCardSprite(52, bot.seat, right);
            var oldX = card.x;
            var oldY = card.y;
            card.x = dealerPosition[0];
            card.y = dealerPosition[1];
            stage.addChild(card);
            createjs.Tween.get(card)
                 .to({ x: oldX, y: oldY }, 100 * speed)
                 .call(playNextAnimation);
            bot.cards[right ? 1 : 0] = card;
        }
    }
    for (var key in bots) {
        if (!bots[key].hasLeft)
            animations.push(giveCard(bots[key], false));
    }

    for (var key in bots) {
        if (!bots[key].hasLeft)
            animations.push(giveCard(bots[key], true));
    }
}

function receiveCard(logger, suite, rank) {
    var cardID = cardNames.indexOf(suite + " " + rank);
    for (var key in bots) {
        if (bots[key].botID == myBotID) {
            var bot = bots[key];
            if (bot.cards[0].currentFrame == 52) {
                var card = createCardSprite(cardID, bot.seat, false);
                stage.removeChild(bot.cards[0]);
                bot.cards[0] = card;

                stage.addChild(card);
            }
            else if (bot.cards[1].currentFrame == 52) {
                var card = createCardSprite(cardID, bot.seat, true);
                stage.removeChild(bot.cards[1]);
                bot.cards[1] = card;

                stage.addChild(card);
            }
        }
    }
}

function rmDealerButton(botID) {
    dealerButtonOwner = null;
    animations.push(function () {
        createjs.Tween.get(dealerButton)
             .to({ x: dealerPosition[0], y: dealerPosition[1] }, 300 * speed)
             .call(function () { dealerButton.visible = false; })
             .call(playNextAnimation);
    });
}

function addDealerButton(botID) {
    dealerButtonOwner = bots[botID];
    dealerButton.visible = true;
    animations.push(function () {
        createjs.Tween.get(dealerButton)
             .to({ x: playerPositions[dealerButtonOwner.seat][0] - dealerButton.getBounds().width / 2, y: playerPositions[dealerButtonOwner.seat][1] - 27 }, 300 * speed)
             .call(playNextAnimation);
    });
}

function flop(cards) {
    animateMessage("Flop");
    function makeFunc(cardID) {
        return function () {
            var card = new createjs.Sprite(cardsSpriteSheet);
            card.gotoAndStop(cardID);
            card.scaleX = card.scaleY = cardScale;
            card.x = cardsOnTablePosition[0] + cardsOnTable.length * (card.getTransformedBounds().width + 3);
            card.y = cardsOnTablePosition[1];
            var oldX = card.x;
            var oldY = card.y;
            card.x = dealerPosition[0];
            card.y = dealerPosition[1];
            stage.addChild(card);
            createjs.Tween.get(card)
                 .to({ x: oldX, y: oldY }, 100 * speed)
                 .call(playNextAnimation);
            cardsOnTable.push(card);
        }
    }
    for (var key in cards) {
        var a = cards[key].split(" ");
        var cardID = cardNames.indexOf(a[0] + " " + a[1]);
        animations.push(makeFunc(cardID));
    }
}

function turn(suite, rank) {
    animateMessage("Turn");
    animations.push(function () {
        var card = new createjs.Sprite(cardsSpriteSheet);
        card.gotoAndStop(cardNames.indexOf(suite + " " + rank));
        card.scaleX = card.scaleY = cardScale;
        card.x = cardsOnTablePosition[0] + cardsOnTable.length * (card.getTransformedBounds().width + 3);
        card.y = cardsOnTablePosition[1];
        var oldX = card.x;
        var oldY = card.y;
        card.x = dealerPosition[0];
        card.y = dealerPosition[1];
        stage.addChild(card);
        createjs.Tween.get(card)
             .to({ x: oldX, y: oldY }, 100 * speed)
             .call(playNextAnimation);
        cardsOnTable.push(card);
    });
}

function river(suite, rank) {
    animateMessage("River");
    animations.push(function () {
        var card = new createjs.Sprite(cardsSpriteSheet);
        card.gotoAndStop(cardNames.indexOf(suite + " " + rank));
        card.scaleX = card.scaleY = cardScale;
        card.x = cardsOnTablePosition[0] + cardsOnTable.length * (card.getTransformedBounds().width + 3);
        card.y = cardsOnTablePosition[1];
        var oldX = card.x;
        var oldY = card.y;
        card.x = dealerPosition[0];
        card.y = dealerPosition[1];
        stage.addChild(card);
        createjs.Tween.get(card)
             .to({ x: oldX, y: oldY }, 100 * speed)
             .call(playNextAnimation);
        cardsOnTable.push(card);
    });
}

function fold(logger) {
    showMessageAtBot(bots[logger], "Fold");

    animations.push(function () {
        createjs.Tween.get(bots[logger].cards[0])
             .to({ x: dealerPosition[0], y: dealerPosition[1] }, 100 * speed);
        createjs.Tween.get(bots[logger].cards[1])
             .to({ x: dealerPosition[0], y: dealerPosition[1] }, 100 * speed)
             .call(removeCards)
             .call(playNextAnimation);

        function removeCards() {
            stage.removeChild(bots[logger].cards[0]);
            stage.removeChild(bots[logger].cards[1]);
            bots[logger].cards = [];
        }
    });
}

function check(logger) {
    showMessageAtBot(bots[logger], "Check");
}

function call(logger, amount) {
    amount = parseInt(amount);
    showMessageAtBot(bots[logger], "Call " + amount);
    bots[logger].setChips(bots[logger].getChips() - amount);
    setChipsOnTable(getChipsOnTable() + amount);
}

function raise(logger, amount) {
    amount = parseInt(amount);
    showMessageAtBot(bots[logger], "Raise " + amount);
    bots[logger].setChips(bots[logger].getChips() - amount);
    setChipsOnTable(getChipsOnTable() + amount);
}

function allin(logger, amount) {
    amount = parseInt(amount);
    showMessageAtBot(bots[logger], "All in " + amount);
    bots[logger].setChips(bots[logger].getChips() - amount);
    setChipsOnTable(getChipsOnTable() + amount);
}

function revealCards(logger, array) {
    var bot = bots[logger];

    for (var i = 0; i < 2; i++) {
        var a = array[i].split(" ");
        var cardID = cardNames.indexOf(a[0] + " " + a[1]);
        var card = createCardSprite(cardID, bot.seat, i != 0);
        stage.removeChild(bot.cards[i]);
        bot.cards[i] = card;
        stage.addChild(card);
    }
    showMessageAtBot(bot, "Reveal cards");
    animations.push(function () {
        window.setTimeout(playNextAnimation, 300 * speed);
    });
}

function showdown() {
    animateMessage("Showdown");
}

function roundWinners(arr) {
    arr.splice(0, 1);
    if (arr.length == 1)
        animateMessage("Round winner: " + arr[0]);
    else
        animateMessage("Round winners: " + arr.join(", "));
}

function handOutPot(arr) {
    animateMessage("Hand out pot");
    for (var i in bots) {
        var bot = bots[i];
        if (bot.name == arr[1]) {
            bot.setChips(bot.getChips() + getChipsOnTable());
            setChipsOnTable(0);
            break;
        }
    }
}

function gameWinner(botID) {
    animateMessage("Game winner: " + bots[botID].name);
}

function collectCards(arr) {
    function removeCard(card) {
        return function () {
            createjs.Tween.get(card)
                 .to({ x: dealerPosition[0], y: dealerPosition[1] }, 100 * speed)
                 .call(removeCardFromStage)
                 .call(playNextAnimation);
            function removeCardFromStage() {
                stage.removeChild(card);
            }
        }
    }

    for (var key in bots)
        for (var k in bots[key].cards)
            animations.push(removeCard(bots[key].cards[k]));

    animations.push(function () {
        for (var key in bots) {
            bots[key].cards = [];
        }
        playNextAnimation();
    });

    for (var key in cardsOnTable) {
        animations.push(removeCard(cardsOnTable[key]));
    }

    animations.push(function () {
        cardsOnTable = [];
        playNextAnimation();
    });
}

function roundEnded() {
    animateMessage("Round ended");
    animations.push(function () {
        window.setTimeout(playNextAnimation, 2000 * speed);
    });
}

function createCardSprite(id, seat, isRight) {
    var card = new createjs.Sprite(cardsSpriteSheet);
    card.gotoAndStop(id);
    card.scaleX = card.scaleY = cardScale;
    if (!isRight)
        card.x = cardPositions[seat][0];
    else
        card.x = cardPositions[seat][0] + card.getTransformedBounds().width + 3;
    card.y = cardPositions[seat][1];
    return card;
}

function leave(botID) {
    var bot = bots[botID];
    animateMessage(bot.name + " left");
    bot.nameText.alpha = 0.4;
    bot.chipsText.alpha = 0.4;
    bot.hasLeft = true;
}

function gotoEvent(eventID) {
    playButton.disabled = true;
    pauseButton.disabled = true;
    speed = 0;
    if (currentEvent == eventID) {
        resetGame();
    }
    targetEvent = eventID;
    running = true;
}

function rebuyOrLeave(botID) {
}

function preflop() {
}

function betRound() {
}

function burn(suite, rank) {
}