<div class="scratchpads-vocabularies-term">
	<div class="scratchpads-vocabularies-term-label">
    <?php echo check_plain($label); ?>
  </div>
  <?php if ($description || $uri || $relation || $wiki): ?>
    <div class="scratchpads-vocabularies-term-other">
      <?php echo $description ? check_plain($description) . '<br/>' : ''?>
      <?php echo $uri ? t('Uri:') . ' ' . l($uri, $uri, array('attributes' => array('target' => '_blank'))) . '<br/>' : ''?>
      <?php echo $relation? t('Relation:') . ' ' . l($relation, $relation, array('attributes' => array('target' => '_blank'))) . '<br/>' : ''?>
      <?php echo $wiki ? t('Discussion:') . ' ' . l($wiki, $wiki, array('attributes' => array('target' => '_blank'))) . '<br/>' : ''?>
      </div>
  <?php endif; ?>
</div>
