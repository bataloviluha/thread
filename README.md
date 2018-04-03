# QDbInteractionThread
constrcut :<br />
QThread thread2;<br />
QDbInteractionThread db_object(&thread2);<br />
db_object.query(<br />
    [/*...*/]//thread2 side<br />
    {<br />
        std::vector<int> retVal;<br />
        //...<br />
        return retVal;<br />
    },<br />
    [/*...*/](auto val)//or std::vector<int>, main thread side<br />
    {<br />
    //...<br />
    });<br />
    <br />
db_object.query(<br />
    [/*...*/]//thread2 side<br />
    {<br />
        std::vector<int> retVal;<br />
        //...<br />
        return retVal;<br />
    },<br />
    [/*...*/](auto val)//or std::vector<int>, main thread side<br />
    {<br />
    //...<br />
    retVal std::list<std::string>();<br />
    }).<br />
then_query(<br />
    [](auto val)//std::list<std::string>, thread2 side<br />
    {<br />
        //...<br />
        //no return<br />
    },<br />
    []//main thread side<br />
    {<br />
    //no params for example<br />
    });<br />