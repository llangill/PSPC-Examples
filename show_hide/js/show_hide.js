(function($, wb) {
  Drupal.behaviors.show_hide = {
    attach: function (context, settings) {
      //Run the Show Hide function.
      //For dataTables, see "sites\all\modules\custom\features\local\local_data\local_data_template\js\showhide.js"
      
      $(document).ready(function(e){
        console.log($(this).closest('.sh-parent'));
        var sh_link_text_start = '<a class="sh-click" id="sh-',
            sh_link_text_2 = '"><span class="sh-controller-info',
            sh_link_text_3 = '"></span><span class="sh-controller-title element-invisible"> ',
            sh_link_text_end = '</span></a>';

        $('.sh-parent.sh-items').each(function(){
          sh_link_text = sh_link_text_start + $(this).attr('id') + sh_link_text_2 + sh_link_text_3 + $(this).find('h3').html() + sh_link_text_end;
          $(this).append(sh_link_text);
        });
        $('.sh-parent.sh-pix').each(function(){
          if ($(this).innerHeight()>'450'){ //PW-20-00913067, PW-20-00913072
            sh_link_text = sh_link_text_start + $(this).attr('id') + sh_link_text_2 + sh_link_text_3 + $(this).find('h3').html() + sh_link_text_end;
            $(this).append(sh_link_text);
          }
        });
        $('.sh-parent.sh-sections h3').each(function(){
          sh_link_text = sh_link_text_start + $(this).attr('id') + sh_link_text_2 + sh_link_text_3 + $(this).html() + sh_link_text_end;
          $(this).append(sh_link_text);
        });
        $('.sh-parent.sh-filters h3').each(function(){
          sh_link_text = sh_link_text_start + $(this).attr('id') + sh_link_text_2 + ' element-invisible' + sh_link_text_3 + $(this).html() + sh_link_text_end;
          $(this).append(sh_link_text);
        });
        $('.sh-click').trigger('click');
      });

      $(document).on('click', '.sh-click', function(e) {
        var sh_this = $(this).closest('.sh-parent');
        if ($(sh_this).hasClass('sh-sections')) {
          sh_less_text = Drupal.t('Click to collapse');
          sh_more_text = Drupal.t('Click to expand');
        } else if ($(sh_this).hasClass('sh-filters')) {
          sh_less_text = Drupal.t('Click to collapse');
          sh_more_text = Drupal.t('Click to expand');
        } else {
          sh_more_text = Drupal.t('Show more');
          sh_less_text = Drupal.t('Show less');
        }
        if (sh_this.hasClass('sh-element-hidden')) {
          sh_this.removeClass('sh-element-hidden');
          sh_this.find('.sh-controller-info').html(sh_less_text);
        }else{
          sh_this.addClass('sh-element-hidden');
          sh_this.find('.sh-controller-info').html(sh_more_text);
        }
      });
    }
  }
})(jQuery);
