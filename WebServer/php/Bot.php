<?php
class Bot{
    public $id;
    public $owner;
    public $name;
    public $lastChangeTime;
    public $code;
    public $codeLang;
    public $state;
}

class BotState{
    const Ok = 0;
    const Processing = 1;
    const Runtime = 2;
    const Compilation = 3;
}