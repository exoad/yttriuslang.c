const { MessageEmbed } = require('discord.js');
const config = require('../../configs/token.json');
const chnl = require('../../configs/chnl.json');
const resource = require('../../configs/resource.json');
module.exports = {
    config: {
        name: `periodic`,
        category: '',
        description: '',
        aliases: [`tableofelements`, `elementstable`, `periodictable`, `ptable`]
    },
    run: async (bot, message, args) => {
      try {
        if(!args[0]) {
            const embed = new MessageEmbed()
            .setTitle("Chemistry Periodic Table")
            .setImage(resource.periodic1)
            .setFooter("If the image isn't showing correctly use \""+config.prefix+"periodic ver\"")

            message.channel.send({embed})
        } else if(args[0] == "ver") {
            const embed = new MessageEmbed()
            .setTitle("Chemistry Periodic Table Versions")
            .setDescription("If the periodic images aren't showing correctly use the following versions of the periodic tables")
            .addField("Usage", "`"+config.prefix+"periodic {version_num}`")
            .addField("Version_Num", "`1, 2, 3, links`")
            .setFooter("Having troubles? Join the support server!")

            message.channel.send({embed})
        } else if(args[0] == "1") {
            const embed = new MessageEmbed()
            .setTitle("Chemistry Periodic Table")
            .setImage(resource.periodic2)
            .setFooter("If the image isn't showing correctly use \""+config.prefix+"periodic ver\"")

            message.channel.send({embed})
        } else if(args[0] == "2") {
            const embed = new MessageEmbed()
            .setTitle("Chemistry Periodic Table")
            .setImage(resource.periodic3)
            .setFooter("If the image isn't showing correctly use \""+config.prefix+"periodic ver\"")

            message.channel.send({embed})
        } else if(args[0] == "3") {
            const embed = new MessageEmbed()
            .setTitle("Chemistry Periodic Table")
            .setImage(resource.periodic4)
            .setFooter("If the image isn't showing correctly use \""+config.prefix+"periodic ver\"")

            message.channel.send({embed})
        } else if(args[0] == "link" || args[0] == "links") {
            const embed = new MessageEmbed()
            .setTitle("Periodic Table Version Links")
            .setDescription("Links List:\n[Link 1](https://schooltutoring.com/help/wp-content/uploads/sites/2/2019/02/Periodic_Table_Of_Elements.svg_.png)\n[Link 2](https://upload.wikimedia.org/wikipedia/commons/4/4d/Periodic_table_large.svg)")

            message.channel.send({embed})
        } else {
            const embed = new MessageEmbed()
            .setTitle("Chemistry Periodic Table")
            .setImage(resource.periodic1)
            .setFooter("If the image isn't showing correctly use \""+config.prefix+"periodic ver\"")

            message.channel.send({embed})
        }
    } catch (err) {
        console.log(err)
        bot.channels.cache.get(chnl.errors).send(`${err.stack}`)
        return message.channel.send(`An internal error was encountered!\nThis has been sent to the dev-logs and will be fixed soon.\n*Sorry for the inconvenience*`);
    }
  }
  }