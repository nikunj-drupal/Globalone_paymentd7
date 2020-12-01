<?php
class CommerceGlobaloneCreateSubscriptionFormat{

  public $_paymentParams;
  public $_postHash;
  public $_terminal;
  public $_postDateTime;
  public $_xml;
  public $_normalizedPaymentParams;
  public $_subinfo;


  public function __construct($terminal,$postDateTime,$subsinfo=array(),$postHash){
    //$this->_paymentParams = $paymentParams;
    //$this->_postHash = $postHash;
    $this->_terminal = $terminal;
    $this->_postDateTime = $postDateTime;
    $this->_postHash = $postHash;
    $this->_subinfo = $subsinfo;
    $this->prepareXML();

    // print('this2::<pre style="color:red;">');
    // print_r($this);
    // print('</pre>');
    //exit;
  }

  private function prepareXML(){

    $xmlStructure = $this->readFields();

    $out = '<?xml version="'.$xmlStructure['XMLHeader']['version'].'" encoding="'.$xmlStructure['XMLHeader']['encoding'].'"?>';

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
      'XMLEnclosureTag' => 'ADDSUBSCRIPTION',
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
          'name' => 'STOREDSUBSCRIPTIONREF',
          'type' => 'text',
          'required' => TRUE,
          'form' => FALSE,
        ),
        array(
          'name' => 'SECURECARDMERCHANTREF',
          'type' => 'text',
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
          'name' => 'STARTDATE',
          'type' => 'text',
          'required' => TRUE,
          'form' => FALSE,
        ),
        // array(
        //   'name' => 'ENDDATE',
        //   'type' => 'text',
        //   'required' => TRUE,
        //   'form' => FALSE,
        // ),
        array(
          'name' => 'EDCCDECISION',
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
    //$params = $this->_paymentParams;
    $out = array();

    $out['MERCHANTREF'] = $this->_terminal['terminal_id'].''.time();
    $out['TERMINALID'] = $this->_terminal['terminal_id'];
    $out['STOREDSUBSCRIPTIONREF'] = $this->_subinfo['storesub_mref'];
    $out['SECURECARDMERCHANTREF'] = $this->_subinfo['seccard_mref'];
    $out['DATETIME'] = $this->_postDateTime;
    $out['STARTDATE'] = date('d-m-Y');
    //$out['ENDDATE'] = '13-07-2019';
    $out['EDCCDECISION'] = 'Y';
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