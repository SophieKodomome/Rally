<h1>Rally &#x1F697; &#x1F525; &#x1F525; &#x1F525;</h1>
<h1>Contexte &#x1F9E0;</h1>
<p>A rally race simulator that data sort the result of a race</p>

<h1>Techno &#x1F4BB;</h1>
<li>Language: <strong>C++</strong></li>
<li>GUI: Qt</li>
<li>Database: Postgre</li>

<h1>Features &#x1F680;</h1>
  <li>A form to write the specials(type of race) & number of racers</li>
  <li>A second form to write the racers and their time (min:sec:millisec)</li>
  <li>The result all group mixed & different group</li>

<h1>To do list &#x1F4CB;</h1>
  <h2> &#x1F4C4; Page Form_1(index)</h2>
    <h3>Front Office</h3>
      <li>Make a form</li>
        <ul>
          <li>Select Special</li>
          <li>Input number of racers(shouldn't be higher than the number of racer in the database</li>
          <li>Button Confirm</li>
        </ul>
    <h3>Back Office</h3>
      <li><u>database</u></li>
      <ul>
        <li>(ok)Special</li>
        <ul>
          <li><i>(ok)Id: Serial</i></li>
          <li><i>(ok)Name: Varchar</i></li>
        </ul>
      </ul>
      <li><u>classes</u></li>
      <ul>
        <li>Special</li>
        <ul>
          <li><i>Id: Int</i></li>
          <li><i>Name: String</i></li>
        </ul>
        <li>PosgreConnect</li>
      </ul>
    <h3>Functions</h3>
      <li>GetConnection()</li>
      <li>GetListSpecial()</li>
      <li>Send datas(Special,Number of Racer) from form to Page Form_2</li>
  <h2> &#x1F4C4; Page Form_2</h2>
    <h3>Front Office</h3>
      <li>Make a Form</li>
        <ul>
          <li>select racers</li>
          <li>input time(min:sec:milisec</li>
          <li>Button Confirm</li>
        </ul>
    <h3>Back Office</h3>
      <li><u>database</u></li>
      <ul>
        <li>(ok)racers</li>
          <ul>
            <li><i>(ok)Id: Serial</i></li>
            <li><i>(ok)Name: Varchar</i></li>
            <li><i>(ok)Time: TimeStamp</i></li>
          </ul>
      </ul>
      <li><u>classes</u></li>
      <ul>
        <li>PosgreConnect</li>
        <li>racers</li>
          <ul>
            <li><i>Id: Serial</i></li>
            <li><i>Name: Varchar</i></li>
            <li><i>Time: TimeStamp</i></li>
          </ul>
      </ul>
    <h3>Functions</h3>
      <li>GetConnection()</li>
      <li>GetListRacers()</li>
      <li>UpdateTimeRacers(id,TimeStamp)</li>
      <li>Send datas(Racers,Time) from form to Page Result</li>
  <h2> &#x1F4C4; Page Result</h2>
    <h3>Front Office</h3>
      <li>List the ranking all group mixed in a table</li>
      <ul>
        <li>Racer name</li>
        <li>Group</li>
        <li>Time</li>
      </ul>
      <li>List the ranking per group in a table</li>
      <ul>
        <li>Racer name</li>
        <li>Time</li>
      </ul>
    <h3>Back Office</h3>
      <li><u>database</u></li>
      <ul>
        <li>Special</li>
        <li>Racers</li>
      </ul>
      <li><u>classes</u></li>
      <ul>
        <li>PosgreConnect</li>
        <li>Special</li>
        <li>Racers</li>
      </ul>
    <h3>Functions</h3>
      <li>GetRankedRacer(all group)</li>
      <li>GetRankedRacer(per group)</li>
