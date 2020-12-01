<?php
class CommerceGlobaloneSubscriptionFormat{

  public $_paymentParams;
  public $_postHash;
  public $_terminal;
  public $_postDateTime;
  public $_xml;
  public $_normalizedPaymentParams;

  public function __construct($paymentParams,$terminal,$postHash,$postDateTime){
    $this->_paymentParams = $paymentParams;
    $this->_postHash = $postHash;
    $this->_terminal = $terminal;
    $this->_postDateTime = $postDateTime;
    $this->prepareXML();
  }

  private function prepareXML(){

    $xmlStructure = $this->readFields();

    $out = '<?xml version="'.$xmlStructure['XMLHeader']['version'].'" encoding="'.$xmlStructure['XMLHeader']['encoding'].'"?>';

    // $out .= '<ADDSUBSCRIPTION><MERCHANTREF>'.time().'</MERCHANTREF><TERMINALID>99108</TERMINALID><SECURECARDMERCHANTREF>991081550130624</SECURECARDMERCHANTREF><DATETIME>'.$this->_postDateTime.'</DATETIME><STARTDATE>01-02-2019</STARTDATE><ENDDATE>31-07-2019</ENDDATE><EDCCDECISION>Y</EDCCDECISION><NEWSTOREDSUBSCRIPTIONINFO><MERCHANTREF>stored-'.time().'</MERCHANTREF><NAME>Animal Life</NAME><DESCRIPTION>Magazine membership</DESCRIPTION><PERIODTYPE>MONTHLY</PERIODTYPE><LENGTH>12</LENGTH><CURRENCY>USD</CURRENCY><RECURRINGAMOUNT>15.87</RECURRINGAMOUNT><INITIALAMOUNT>10.99</INITIALAMOUNT><TYPE>AUTOMATIC</TYPE><ONUPDATE>CONTINUE</ONUPDATE><ONDELETE>CANCEL</ONDELETE></NEWSTOREDSUBSCRIPTIONINFO><HASH>'.$this->_postHash.'</HASH></ADDSUBSCRIPTION>';

    $out .= '<'.$xmlStructure['XMLEnclosureTag'].'>';

    $params = $this->preparePaymentParameter();

    foreach($params as $key=>$param):
      $tag = strtoupper($key);
    $out .= '<'.$tag.'>'.$param.'</'.$tag.'>';
    endforeach;
    $out .= '</'.$xmlStructure['XMLEnclosureTag'].'>';

     $this->_xml = $out;
     return $out;
  }

 protected function readFields() {
    return array(
      'XMLHeader' => array(
        'version' => '1.0',
        'encoding' => 'UTF-8',
      ),
      'XMLEnclosureTag' => 'ADDSTOREDSUBSCRIPTION',
      'Fields' => array(
        array(
          'name' => 'MERCHANTREF',
          'type' => 'text',
          'required' => TRUE,
          'form' => FALSE,
        ),
        array(
          'name' => 'TERMINALID',
          'type' => 'number',
          'required' => TRUE,
          'form' => FALSE,
        ),
        array(
          'name' => 'DATETIME',
          'type' => 'datetime',
          'required' => TRUE,
          'format' => 'Y-m-dH:i:s:u',
          'form' => FALSE,
        ),
        array(
          'name' => 'NAME',
          'type' => 'text',
          'minlenght' => 16,
          'maxlenght' => 16,
          'required' => TRUE,
          'form' => FALSE,
        ),
        array(
          'name' => 'DESCRIPTION',
          'type' => 'text',
          'required' => TRUE,
          'form' => FALSE,
        ),
        array(
          'name' => 'PERIODTYPE',
          'type' => 'number',
          'required' => TRUE,
          'form' => FALSE,
        ),
        array(
          'name' => 'LENGTH',
          'type' => 'number',
          'required' => TRUE,
          'form' => FALSE,
        ),
        array(
          'name' => 'CURRENCY',
          'type' => 'text',
          'required' => TRUE,
          'form' => FALSE,
        ),
        array(
          'name' => 'RECURRINGAMOUNT',
          'type' => 'number',
          'required' => TRUE,
          'form' => FALSE,
        ),
        array(
          'name' => 'INITIALAMOUNT',
          'type' => 'number',
          'required' => TRUE,
          'form' => FALSE,
        ),
        array(
          'name' => 'TYPE',
          'type' => 'text',
          'required' => TRUE,
          'form' => FALSE,
        ),
        array(
          'name' => 'ONUPDATE',
          'type' => 'text',
          'required' => TRUE,
          'form' => FALSE,
        ),
        array(
          'name' => 'ONDELETE',
          'type' => 'text',
          'required' => TRUE,
          'form' => FALSE,
        ),
        array(
          'name' => 'HASH',
          'type' => 'text',
          'required' => TRUE,
          'form' => FALSE,
        ),
      ),
    );
  }


  private function preparePaymentParameter(){
    $params = $this->_paymentParams;
    $out = array();

    $out['MERCHANTREF'] = $this->_terminal['terminal_id'].''.time();
    $out['TERMINALID'] = $this->_terminal['terminal_id'];
    $out['DATETIME'] = $this->_postDateTime;
    $out['NAME'] = $params['CARDHOLDERNAME'];
    $out['DESCRIPTION'] = 'WIDEN-SUBSCRIPTION';
    $out['PERIODTYPE'] = 'MONTHLY';
    $out['LENGTH'] = $params['LENGTH'];
    $out['CURRENCY'] = 'USD';
    $out['RECURRINGAMOUNT'] = $params['RECURRINGAMOUNT'];
    $out['INITIALAMOUNT'] = $params['INITIALAMOUNT'];
    $out['TYPE'] = 'AUTOMATIC';
    $out['ONUPDATE'] = 'CONTINUE';
    $out['ONDELETE'] = 'CANCEL';
    $out['HASH'] = $this->_postHash;

    $this->_normalizedPaymentParams = $out;

    return $out;
  }

  private function cleanCardNumber($cardNumber = ''){
    $cardNumber = str_replace('-' , '', $cardNumber);
    $cardNumber = str_replace(' ' , '', $cardNumber);

    return $cardNumber;
  }

  private function cleanExpiryDate($month = '',$year = ''){
    if(strlen($year)>2):
      $year = substr($year, 2,2);
    endif;

    $date = $month.$year;

    return $date;
  }

  public function getNormalizedPaymentParams(){
    return $this->_normalizedPaymentParams;
  }

  public function getXML(){
    return $this->_xml;
  }

}