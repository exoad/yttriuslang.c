const { MessageEmbed } = require('discord.js');
const config = require('../../configs/token.json');
const chnl = require('../../configs/chnl.json');
const resource = require('../../configs/resource.json');
const superagent = require('superagent');
module.exports = {
    config: {
        name: `support`,
        category: '',
        description: '',
        aliases: [`supportserver`]
    },
    run: async (bot, message, args) => {
      try {
        const embed = new MessageEmbed()
        .setTitle("Support")
        .setDescription("Have questions regarding usage of the bot? Come join the discord!")
        .addField("Main Discord Support Server", "[Invite Link]("+chnl.spprt_server+")")
        .addField("Backup Invite Link", "[Backup Link]("+chnl.bckup_link+")")

        message.channel.send({embed})
    } catch (err) {
      const embed = new MessageEmbed()
      .setTitle("Whoops, looks like the requested command doesn't exist (yet)!")
      .setThumbnail(resource.aw_snap)
      .setDescription("Use `"+config.prefix+"help` for a list of avaliable commands!")
      .setFooter("Still facing issues? Join the support server!")
        message.channel.send({ embed }).then(m => {
            m.delete({ timeout: 5000 })
        });
    }
  }
  }