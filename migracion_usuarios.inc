<?php
// use Migration class
class nnmxUserMigration extends Migration {
	public function __construct() {
	parent::__construct(MigrateGroup::getInstance('nnmx group'));
    $this->description = t('Migrate Drupal 6 users from nnmx old installation');


// some field definitions; roles will be added later in prepareRow()
  $source_fields = array(
    'uid' => t('User ID'),
    'roles' => t('The set of roles assigned to a user.'),
  );

  // only import active users from the old db
    $query = db_select(SOURCE_DATABASE .'.users', 'u')
      ->fields('u', array('uid', 'name', 'pass', 'mail', 'created', 'access', 'login', 'status', 'init'))
      //change conditios let the inactive and remove never login users
      //->condition('u.status', 1, '=')
      ->condition('u.uid', 1, '>')
      ->condition('u.login', 0, '<>')
      ->orderBy('u.uid', 'ASC');
    $this->source = new MigrateSourceSQL($query, $source_fields);

    //set destination
    $this->destination = new MigrateDestinationUser(array('md5_passwords' => TRUE));

    //mapping of id from rollback task
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'uid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'D6 Unique User ID',
          'alias' => 'u',
        )
      ),
      MigrateDestinationUser::getKeySchema()
    );


    //listing fields mapping
    $this->addFieldMapping('is_new')->defaultValue(TRUE);
    $this->addFieldMapping('uid', 'uid');
    $this->addFieldMapping('name', 'name')->dedupe('users', 'name');
    $this->addFieldMapping('pass', 'pass');
    $this->addFieldMapping('mail', 'mail')->dedupe('users', 'mail');
    $this->addFieldMapping('language')->defaultValue('');
    $this->addFieldMapping('theme')->defaultValue('');
    $this->addFieldMapping('signature')->defaultValue('');
    $this->addFieldMapping('signature_format')->defaultValue('filtered_html');
    $this->addFieldMapping('created', 'created');
    $this->addFieldMapping('access', 'access');
    $this->addFieldMapping('login', 'login');
    $this->addFieldMapping('status', 'status');
    $this->addFieldMapping('picture')->defaultValue(0);
    $this->addFieldMapping('init', 'init');
    $this->addFieldMapping('timezone')->defaultValue(NULL);
    $this->addFieldMapping('path')->issueGroup(t('DNM')); // DNM = Do Not Map
    $this->addFieldMapping('roles', 'roles');

  }//end of constructor


  public function prepareRow($current_row) {
    $source_id = $current_row->uid;
    $query = db_select(SOURCE_DATABASE .'.users_roles', 'r')
      ->fields('r', array('uid', 'rid'))
      ->condition('r.uid', $source_id, '=');
    $results = $query->execute();
    // add the Authenticated role for everyone
    $roles = array('2' => '2');
    foreach ($results as $row) {
      // adjust the old to new role id's
      // Editors (rid 6) need role id 4
      if ($row->rid == 6) {
        $roles['4'] = '4';
      }
      // Reportero (rid 12) need role id 5
      if ($row->rid == 12) {
        $roles['5'] = '5';
      }
      // admin (rid 13) need role id 3
      if ($row->rid == 13) {
        $roles['3'] = '3';
      }
      // mesa (rid 11) need role id 8
      if ($row->rid ==11) {
        $roles['8'] = '8';
      }
      // oax (rid 16) need role id 6
      if ($row->rid == 16) {
        $roles['6'] = '6';
      }
      // sjbt (rid 17) need role id 7
      if ($row->rid == 17) {
        $roles['7'] = '7';
      }
      // cc (rid 18) need role id 9
      if ($row->rid == 18) {
        $roles['9'] = '9';
      }
      // Collaboraters are role id 5 in both old and new dbs
      //$roles[$row->rid] = $row->rid;
    }
    $current_row->roles = $roles;
    return TRUE;
    // return FALSE if you wish to skip a particular row
  }


}
