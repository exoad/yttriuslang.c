const { MessageEmbed } = require('discord.js');
const config = require('../../configs/config.json');
const chnl = require('../../configs/chnl.json');
const resource = require('../../configs/resource.json');
const superagent = require('superagent');
const colors = require('../../configs/colors.json');
const ready = require('../../events/client/ready');
module.exports = {
    config: {
        name: `restart`,
        category: '',
        description: '',
        aliases: [`reboot`]
    },
    run: async (bot, message, args) => {
    try {
        if(message.author.id == config.owner) {
            message.channel.send("Destroying current session...").then(m => 
                bot.destroy()
            ).then(t => {
                bot.login(config.TOKEN);

                bot.user.setPresence({ activity: { name: toString(ready.status) }, status: "dnd" });
                message.channel.send("Files Destroyed. System restarted")
            })
        }

    } catch (err) {
        const embed = new MessageEmbed()
        .setTitle("Whoops, looks like something went wrong!")
        .setThumbnail(resource.aw_snap)
        .setDescription("Use `"+config.prefix+"help` for a list of avaliable commands or use `"+config.prefix+"support` to join the support server!")
        .setFooter("Still facing issues? Join the support server!")
        message.channel.send({ embed }).then(m => {
            m.delete({ timeout: 5000 })
        });
    }
  }
  }